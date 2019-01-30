#include "Exit.h"

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
	char *m = "Error: ";
	char *msg = m + errorcode;
	server->sendMessage(1, msg);
}

Exit::~Exit()
{
}
