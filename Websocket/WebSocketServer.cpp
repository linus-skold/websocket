#include "WebSocketServer.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wincrypt.h>
#else

#endif

#include "SHA1.hpp"

#include <vector>

namespace ws
{
	constexpr char* S_HTTP_REQUEST = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
	constexpr char* S_MAGIC_STRING = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

	WebSocketServer::WebSocketServer(uint16_t port)
	{
		m_Listener.Bind(port, [&](SOCKET socket) { WebSocketServer::ConnectionAccepted(socket); });
	}

	WebSocketServer::~WebSocketServer()
	{
	}

	void WebSocketServer::ConnectionAccepted(SOCKET socket)
	{
		
		printf("Connection Accepted!\n");

		constexpr int SIZE_OF_MSG = 1024;
		char recvbuf[SIZE_OF_MSG];
		int recvbuflen = SIZE_OF_MSG;

		do
		{
			int iSendResult = 0;
			ZeroMemory(recvbuf, SIZE_OF_MSG);
			int32_t recv_size = recv(socket, recvbuf, recvbuflen, 0);

			if (recv_size > 0)
			{
				printf("bytes received : %d\n", recv_size);

				printf("%s\n", recvbuf);

				std::string key = GetRequestKey(recvbuf);
				printf("%s\n", key.c_str());

				std::string accept_key;
				accept_key = CreateAcceptKey(key);
				//printf("%s\n", accept_key.c_str());

				std::string http_request; 
				http_request += S_HTTP_REQUEST;
				http_request += accept_key;
				http_request += "\r\n\r\n";

				std::vector<char> request_buffer;

				for (auto& c : http_request)
				{
					request_buffer.push_back(c);
				}

				iSendResult = send(socket, &request_buffer[0], request_buffer.size(), 0);
				if (iSendResult == SOCKET_ERROR)
				{
					//printf("send failed with error %s\n", LastErr().c_str());
					//close_cleanup_client();
				}
				printf("bytes sent %d\n", iSendResult);
				break;
			}
			else if (recv_size == 0)
			{
				//	printf("connection closing...\n");
			}
			else
			{
				//printf("recv failed with error : %s\n", LastErr().c_str());
			}

		} while (true);

	}

	std::string WebSocketServer::CreateAcceptKey(std::string request_key) const
	{
		uint8_t binary[20] = { 0 };
		request_key += S_MAGIC_STRING;

		HashSha1(request_key, binary);
		return EncodeBase64(binary);
	}

	std::string WebSocketServer::EncodeBase64(uint8_t* byte) const
	{
#ifdef _WIN32
		DWORD length = 0;
		CryptBinaryToStringA(byte, 20, CRYPT_STRING_BASE64, nullptr, &length);
		char* str = new char[length];
		memset(str, 0, length);
		CryptBinaryToStringA(byte, 20, CRYPT_STRING_BASE64, str, &length);
#endif
		std::string key;
		key += str;

		delete str;
		str = nullptr;
		return key;
	}

	void WebSocketServer::HashSha1(const std::string& request_key, uint8_t* out_binary) const
	{
		SHA1 checksum;

		checksum.update(request_key);
		std::string result = checksum.final();
		uint8_t* byte_list = checksum.get_bytelist();
		uint32_t* digest = checksum.get_digest();

		uint8_t byte[20] = { 0 };
		for (int i = 0; i < 5; i++)
		{
			byte[(i * 4) + 3] = digest[i] & 0x000000ff;
			byte[(i * 4) + 2] = (digest[i] & 0x0000ff00) >> 8;
			byte[(i * 4) + 1] = (digest[i] & 0x00ff0000) >> 16;
			byte[(i * 4) + 0] = (digest[i] & 0xff000000) >> 24;
		}

		memcpy(out_binary, byte, 20);
	}

	std::string WebSocketServer::GetRequestKey(const char* buffer) const
	{
		std::string locBuffer(buffer);
		const size_t end_pos = locBuffer.find("==");
		const size_t pos = locBuffer.rfind(":", end_pos);
		return locBuffer.substr(pos + 2, end_pos - pos);
	}

};