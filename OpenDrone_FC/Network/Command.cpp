#include "Command.h"
#include <string>


Command::Command(long timestamp, int droneID, std::string cmd) {
	this->timestamp = timestamp;
	this->droneID = 1;
	this->command = cmd;
}


Command::~Command()
{
}
