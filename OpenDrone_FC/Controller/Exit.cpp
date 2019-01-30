#include "Exit.h"
#include <string.h>
#include <sstream>

using namespace std;

Exit* Exit::instance = 0;

Exit::Exit()
{
	server = TCPServer::getInstance();
}


Exit* Exit::getInstance()
{
	if (instance == 0)
	{
		instance = new Exit();
	}

	return instance;
}

void Exit::sendError(int errorcode, bool stopFC) {
	stringstream ss;
	ss << "255;Error: " << errorcode;
	char *str = (char*)(ss.str().c_str());

	server->sendMessage(str);
}

Exit::~Exit()
{
}
