#include "../Filter/Filter.h"
#pragma once
class UltraSonic
{
public:
	int pin_trigger;
	int pin_echo;
	int id;
	Filter *filter;

	UltraSonic(int pin_trigger, int pin_echo, int num);
	~UltraSonic();
	float distance();
	int getId();
};
