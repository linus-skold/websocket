//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-------+-+-------------+-------------------------------+
//|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
//|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
//|N|V|V|V|       |S|             |   (if payload len==126/127)   |
//| |1|2|3|       |K|             |                               |
//+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
//|     Extended payload length continued, if payload len == 127  |
//+ - - - - - - - - - - - - - - - +-------------------------------+
//|                               |Masking-key, if MASK set to 1  |
//+-------------------------------+-------------------------------+
//| Masking-key (continued)       |          Payload Data         |
//+-------------------------------- - - - - - - - - - - - - - - - +
//:                     Payload Data continued ...                :
//+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
//|                     Payload Data continued ...                |
//+---------------------------------------------------------------+

#include <iostream>
#include <string.h>
#include <vector>



std::string get_key(const char* buffer)
{
	std::string locBuffer(buffer);
	size_t end_pos = locBuffer.find("==");
	size_t pos = locBuffer.rfind(":", end_pos);
	std::string b64Hash = locBuffer.substr(pos + 2, end_pos - pos);
	return b64Hash;
}






int main()
{


	std::vector<char> http_request_buffer;
	size_t header_len = strlen(http_request);

	for (size_t i = 0; i < header_len; ++i)
	{
		http_request_buffer.push_back(http_request[i]);
	}


	WSADATA wsaData;
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET client_socket = INVALID_SOCKET;

	struct addrinfo * result = NULL;
	struct addrinfo hints;

	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		printf("failed with err %d\n", err);
		std::getchar();
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;




	err = getaddrinfo(nullptr, "8091", &hints, &result);
	if (err != 0)
	{
		printf("failed with err %d\n", err);
		WSACleanup();
		std::getchar();
		return 1;
	}

	listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		printf("failed to create listen socket %s\n", LastErr().c_str());
		WSACleanup();
		freeaddrinfo(result);
		WSACleanup();
		std::getchar();
		return 1;
	}

	err = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (err == SOCKET_ERROR)
	{
		printf("failed to bind socket %s\n", LastErr().c_str());
		freeaddrinfo(result);
		closesocket(listen_socket);
		WSACleanup();
		std::getchar();
		return 1;
	}

	freeaddrinfo(result);

	err = listen(listen_socket, SOMAXCONN);
	if (err == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}


	client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		printf("accept failed with err : %s\n", LastErr().c_str());
		closesocket(listen_socket);
		WSACleanup();
		std::getchar();
		return 1;
	}

	closesocket(listen_socket);

	constexpr int SIZE_OF_MSG = 1024;
	char recvbuf[SIZE_OF_MSG];
	int recvbuflen = SIZE_OF_MSG;

	auto close_cleanup_client = [&]() {
		closesocket(client_socket);
		WSACleanup();
	};

	int iSendResult = 0;
	do
	{
		ZeroMemory(recvbuf, SIZE_OF_MSG);
		err = recv(client_socket, recvbuf, recvbuflen, 0);

		if (err > 0)
		{
			printf("bytes received : %d\n", err);

			printf("%s\n", recvbuf);

			std::string key = get_key(recvbuf);
			printf("%s\n", key.c_str());

			std::string accept_key = create_ws_key(key.c_str());
			printf("%s\n", accept_key.c_str());
			for (size_t i = 0; i < accept_key.length(); ++i)
			{
				http_request_buffer.push_back(accept_key[i]);
			}
			http_request_buffer.push_back('\r');
			http_request_buffer.push_back('\n');
			http_request_buffer.push_back('\r');
			http_request_buffer.push_back('\n');

			iSendResult = send(client_socket, &http_request_buffer[0], http_request_buffer.size(), 0);
			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error %s\n", LastErr().c_str());
				close_cleanup_client();
				std::getchar();
				return 1;
			}
			printf("bytes sent %d\n", iSendResult);
		}
		else if (err == 0)
		{
			//	printf("connection closing...\n");
		}
		else
		{
			printf("recv failed with error : %s\n", LastErr().c_str());
			close_cleanup_client();
			std::getchar();
			return 1;
		}

	} while (true);


	err = shutdown(client_socket, SD_SEND);
	if (err == SOCKET_ERROR)
	{
		printf("shutdown failed with error %s\n", LastErr().c_str());
		close_cleanup_client();
		std::getchar();
		return 1;
	}

	close_cleanup_client();

	std::getchar();

	return 0;
}