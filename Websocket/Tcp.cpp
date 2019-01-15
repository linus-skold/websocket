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
#include <string>

#define SOCKET_ASSERT(condition, msg) \
		char temp[512] = { 0 };\
		sprintf_s(temp, "%s, error : %s\n", msg, GetLastError());\
		assert(condition && temp);


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

	Tcp::~Tcp()
	{
		m_IsRunning = false;
	}


	void Tcp::Bind(uint16_t port, FAcceptCallback callback)
	{
#ifdef _WIN32
		WSADATA wsaData;
		int32_t error = WSAStartup(MAKEWORD(2, 2), &wsaData);
		SOCKET_ASSERT(error == 0, "Failed to setup WSAData!");
#endif

		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		struct addrinfo* result = nullptr;
		error = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &result);
		if (error != 0)
		{
			SOCKET_ASSERT(error == 0, "getaddrinfo() failed to get hints & result");
			Cleanup();
			return;
		}

		//setup listener
		m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (m_Socket == INVALID_SOCKET)
		{
			SOCKET_ASSERT(m_Socket, "failed to create listen socket");
			freeaddrinfo(result);
			Cleanup();
			return;
		}

		error = bind(m_Socket, result->ai_addr, (int)result->ai_addrlen);
		if (error == SOCKET_ERROR)
		{
			SOCKET_ASSERT(error != SOCKET_ERROR, "failed to bind socket");
			freeaddrinfo(result);
			closesocket(m_Socket);
			Cleanup();
			return;
		}

		freeaddrinfo(result);

		error = listen(m_Socket, SOMAXCONN);
		if (error == SOCKET_ERROR)
		{
			SOCKET_ASSERT(error != SOCKET_ERROR, "listen failed");
			closesocket(m_Socket);
			Cleanup();
		}

		m_IsRunning = true;

		m_ListenerThread = std::thread([&, callback]() {

			while (m_IsRunning)
			{
				SOCKET client_socket = INVALID_SOCKET;
				client_socket = accept(m_Socket, NULL, NULL);
				SOCKET_ASSERT(client_socket != INVALID_SOCKET, "Failed to accept connection!");

				if (client_socket != INVALID_SOCKET)
				{
					callback(client_socket);
				}
			}


		});
		m_ListenerThread.detach();

	}

	void Tcp::Cleanup()
	{
#ifdef _WIN32
		WSACleanup();
#endif

	}

};