#include "Filter.h"
#include <iostream>
#include <list> 
#include <iterator> 

using namespace std;

list <double> list1;
double MAX_VALUE, MIN_VALUE, MIN_SIZE;


Filter::Filter(double maxValue, double minValue, double minSize)
{
	this->MAX_VALUE = maxValue;
	this->MIN_VALUE = minValue;
	this->MIN_SIZE = minSize;
}


Filter::~Filter()
{
}


double Filter::addValue(double value)
{	
	if (value <= MAX_VALUE && value > MIN_VALUE)
	{
		list1.push_back(value);
	}

	if (list1.size() > MIN_SIZE)
	{
		list1.pop_front();
	}

	double num;
	list <double> ::iterator i;
	for (i = list1.begin(); i != list1.end(); ++i)
	{
		num = num + *i;
	}

	if (list1.size() > 0)
	{
		return num / list1.size();
	}
	else
	{
		return -1.0;
	}
}