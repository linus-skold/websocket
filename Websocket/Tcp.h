#pragma once

namespace Socket
{

	typedef void(*FSocketAccept)();
	class Tcp
	{
	public:
		Tcp(const char* listen_to_port, FSocketAccept accpet_callback);
		~Tcp();

	};
};