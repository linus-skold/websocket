#pragma once
#include <cstdint>
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
		WebSocketServer(const char* ip_and_port);
		~WebSocketServer();

		//I need a callback on connect

	private:

		const char* CreateAcceptKey(const char* request_key);
		const char* EncodeBase64(uint8_t* byte);
		uint8_t* HashSha1(const char* request_key);
		void FlipByteOrder(uint8_t* byte, uint32_t* digest);
		
	protected:


	};

};