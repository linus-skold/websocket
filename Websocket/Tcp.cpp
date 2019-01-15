#include "Tcp.h"
#include <cassert>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#endif

#include <cstdio>

namespace Socket
{
	const char* GetLastError()
	{
		char* str = nullptr;

#ifdef _WIN32
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&str, 0, NULL);
#endif
		return str;
	}

	Tcp::Tcp(const char* listen_to_port, FAcceptCallback accept_callback)
	{
		WSADATA wsaData;
		SOCKET listen_socket = INVALID_SOCKET;
		SOCKET client_socket = INVALID_SOCKET;

		struct addrinfo * result = NULL;
		struct addrinfo hints;

		int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
		assert(err == 0 && "Failed to setup WSAData");

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		err = getaddrinfo(nullptr, listen_to_port, &hints, &result);
		if (err != 0)
		{
			//printf("failed with err %d\n", err);
			assert(err == 0 && "getaddrinfo() failed to get hints & result");
			WSACleanup();
		}

		//setup listener
		listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listen_socket == INVALID_SOCKET)
		{
			//printf("failed to create listen socket %s\n", LastErr().c_str());
			WSACleanup();
			freeaddrinfo(result);
			WSACleanup();
		}

		err = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
		if (err == SOCKET_ERROR)
		{
			//printf("failed to bind socket %s\n", LastErr().c_str());
			freeaddrinfo(result);
			closesocket(listen_socket);
			WSACleanup();
		}

		freeaddrinfo(result);

		//
		err = listen(listen_socket, SOMAXCONN);
		if (err == SOCKET_ERROR)
		{
			char _error[500];
			sprintf_s(_error, "listen failed with error: %s\n", GetLastError());
			assert(err != SOCKET_ERROR && _error);
			closesocket(listen_socket);
			WSACleanup();
		}

		//end setup listener

		//wait for connection, this can be put on a thread? so it's non-blocking? 
		client_socket = accept(listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
		{
			//printf("accept failed with err : %s\n", LastErr().c_str());
			closesocket(listen_socket); // should not be removed immediately. Max connection and stuffs
			WSACleanup();
		}

		closesocket(listen_socket);
	}


	Tcp::~Tcp()
	{
	}

};