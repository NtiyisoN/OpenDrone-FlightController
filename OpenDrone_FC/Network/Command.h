#pragma once
#include "iostream";
#include <string>;
#include <iostream>;

class Command
{
public:
	Command(long timestamp, int droneID, std::string cmd);
	~Command();

	long timestamp;
	int droneID = 1;
	std::string command;
};

