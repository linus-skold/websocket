#pragma once
#include <cstdint>
#include "Tcp.h"

namespace ws
{
	struct WebsocketHeader
	{
		union
		{
			struct
			{
				uint32_t FIN : 1;
				uint32_t RSV1 : 1;
				uint32_t RSV2 : 1;
				uint32_t RSV3 : 1;
				uint32_t OPCODE : 4;
				uint32_t MASK : 1;
				uint32_t PAYLOAD : 7;
			};
			uint16_t HEADER;
		};
	};

	class WebSocketServer
	{
	public:
		WebSocketServer(uint16_t port);
		~WebSocketServer();

		//I need a callback on connect
		
	private:
		void ConnectionAccepted(SOCKET socket);
		
		Socket::Tcp m_Listener; //will listen for new connections


		std::string CreateAcceptKey(std::string request_key) const;
		std::string EncodeBase64(uint8_t* byte) const;
		void HashSha1(const std::string& request_key, uint8_t* out_binary) const;
		std::string GetRequestKey(const char* buffer) const;

	protected:


	};

};