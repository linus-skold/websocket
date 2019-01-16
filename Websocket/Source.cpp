#include <iostream>
#include <string.h>
#include <vector>
#include "WebSocketServer.h"

int main()
{
	ws::WebSocketServer server(8091);

	std::getchar();

	return 0;
}

