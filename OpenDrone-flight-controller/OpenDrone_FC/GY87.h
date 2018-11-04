#pragma once
class GY87
{
public:
	GY87();
	void getGyroValues(double *ar);
	void getAccValues(double *ar);
	void getBarometerValues(double *ar);
	~GY87();
};

