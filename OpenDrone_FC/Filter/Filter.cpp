/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Tim Klecka
 * 	@version 0.0.1 07.01.2019
 */
#include "Filter.h"
#include <iostream>
#include <list> 
#include <iterator> 
using namespace std;

/**
	Filter
	@param double minValue, double maxValue, double minSize
*/
Filter::Filter(double minValue, double maxValue, double minSize)
{
	this->maxValue = maxValue;
	this->minValue = minValue;
	this->minSize = minSize;
}

/**
	adds a value to the Filtered array
	@param double value
	@return double

	@info
	* returns the filtered array
*/
double Filter::addValue(double value)
{	
	if (value <= maxValue && value > minValue)
	{
		this->list1.push_back(value);
	}

	if (this->list1.size() > minSize)
	{
		this->list1.pop_front();
	}

	double num;
	list <double> ::iterator i;
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