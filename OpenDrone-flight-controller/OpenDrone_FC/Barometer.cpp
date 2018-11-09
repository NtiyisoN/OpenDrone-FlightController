#include "Barometer.h"
#include <bmp180.h>
#include <wiringPi.h>

Barometer::Barometer()
{
	bmp180Setup(200);
}

{
	ar[0] = analogRead(200 + 0); //Temperatur
	ar[1] = analogRead(200 + 1); //Pressure - hPa
}

Barometer::~Barometer()
{
}
