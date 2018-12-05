/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 */

#include <list> 
#pragma once
class Filter
{
public:
	std::list<float> list1;
	float maxValue, minValue, minSize;

	Filter(float minValue, float maxValue, float minSize);
	~Filter();
	float addValue(float value);
};
