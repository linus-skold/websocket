#pragma once
#include <functional>

namespace Socket
{

	typedef std::function<void()> FAcceptCallback;

	class Tcp
	{
	public:
		Tcp() = default;
		Tcp(const char* listen_to_port, FAcceptCallback accept_callback);
		~Tcp();
	
	private:
		FAcceptCallback m_AcceptCallback;


	};
};