#pragma once
class Filter
{
public:
	Filter(double maxValue, double minValue, double minSize);
	~Filter();
	double addValue(double value);
	int MAX_VALUE, MIN_VALUE, MIN_SIZE;
};

