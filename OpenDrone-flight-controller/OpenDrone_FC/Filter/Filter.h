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

