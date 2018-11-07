#pragma once
class Magnetometer
{
public:
	Magnetometer();
	void getMagnetometerValues(double *ar);
	~Magnetometer();
private:
	int fdMagnet;
	int addressMag;
};

