#pragma once

#include <functional>
#include <thread>
#ifdef _WIN32
#include <WinSock2.h>
#endif
namespace Socket
{

	typedef std::function<void(SOCKET)> FAcceptCallback;

	class Tcp
	{
	public:
		Tcp() = default;
		~Tcp();
	
		void Bind(uint16_t port, FAcceptCallback callback);

	private:
		volatile bool m_IsRunning = false;

		void Cleanup();
		SOCKET m_Socket;

	};
};