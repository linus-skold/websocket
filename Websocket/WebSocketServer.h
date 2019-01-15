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
		
		void ConnectionAccepted(SOCKET socket);
	private:
		
		Socket::Tcp m_Listener; //will listen for new connections


		const char* CreateAcceptKey(const char* request_key);
		const char* EncodeBase64(uint8_t* byte);
		void HashSha1(const char* request_key, uint8_t* out_binary);
		void FlipByteOrder(uint8_t* byte, uint32_t* digest);
		
	protected:


	};

};