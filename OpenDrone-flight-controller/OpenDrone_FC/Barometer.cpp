#include "Barometer.h"
#include <bmp180.h>

Barometer::Barometer()
{
	bmp180Setup(200);
}

void Barometer::getBarometerValues(double *ar)
{
	ar[0] = analogRead(200 + 0); //Temperatur - ??
	ar[1] = analogRead(200 + 1); //Pressure - hPa
}


Barometer::~Barometer()
{
}
