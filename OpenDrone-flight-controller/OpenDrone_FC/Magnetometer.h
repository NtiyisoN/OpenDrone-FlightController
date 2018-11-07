#pragma once
class Magnetometer
{
	int addressMag = 0x1E;
	int fdMagnet;

public:
	Magnetometer();
	void getMagnetometerValues(double *ar);
	~Magnetometer();
};

