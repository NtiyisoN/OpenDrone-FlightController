#include "Filter.h"
#include <iostream>
#include <list> 
#include <iterator> 
using namespace std;

Filter::Filter(float minValue, float maxValue, float minSize)
{
	this->maxValue = maxValue;
	this->minValue = minValue;
	this->minSize = minSize;
}

float Filter::addValue(float value)
{	
	if (value <= maxValue && value > minValue)
	{
		this->list1.push_back(value);
	}

	if (this->list1.size() > minSize)
	{
		this->list1.pop_front();
	}

	float num;
	list <float> ::iterator i;
	for (i = this->list1.begin(); i != this->list1.end(); ++i)
	{
		num = num + *i;
	}

	if (this->list1.size() > 0)
	{
		return num / this->list1.size();
	}
	else
	{
		return -1.0;
	}
}

Filter::~Filter()
{
}