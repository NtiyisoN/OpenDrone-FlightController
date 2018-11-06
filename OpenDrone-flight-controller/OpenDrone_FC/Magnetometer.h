#pragma once
class Magnetometer
{
public:
	Magnetometer();
	double *getMagnetometerValues();
	~Magnetometer();
private:
	int fdMagnet;
	int addressMag;
};

