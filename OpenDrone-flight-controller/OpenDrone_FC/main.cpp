#include "UltraSonic.h"
#include "Barometer.h"
#include "GyroAccelerometer.h"
#include "Magnetometer.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <wiringPi.h>
using namespace std;

void *runUltrasonic(void *interval)
{
	//Initializing the wiringPi-Gpio's
	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	//Initialize the sensors
	int len = 2;
	UltraSonic* sensors[len];
	sensors[0] = new UltraSonic(17, 27, 1);
	sensors[1] = new UltraSonic(17, 27, 1);

	//Infinite loop to keep measuring --> TODO: Need to be changed
	double curDistance;
	while (1) 
	{
		for (int i = 0; i < len; i++) 
		{
			curDistance = sensors[i]->distance();
			cout << "Ultrasonic " << sensors[i]->getId() << ": Distance: " << curDistance << "\n";
			delay(2);
		}
		delay((int)interval);
	}
}

void *runGyroAccelerometer(void *interval)
{
	//Initializing the sensor
	GyroAccelerometer* gyroAcc = new GyroAccelerometer();
	ofstream myfile;
	myfile.open("example.csv");
	
	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		float *values = gyroAcc->getValues();
		cout << "Acc x=" << values[1] << " y=" << values[2] << " z=" << values[3];
		cout << "Gyrometer x=" << values[4] << " y=" << values[5] << " z=" << values[6] << "\n";
		myfile << values[0] << ";" << values[1] << ";" << values[2] << ";" << values[3] << ";" << values[4] << ";" << values[5] << ";" << values[6] << ";\n";
		delay((int)interval);
	}

	myfile.close();
}

void *runBarometer(void *interval)
{
	//Initializing the sensor
	Barometer *barometer = new Barometer();
	double values[2];

	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		barometer->getBarometerValues(values);
		cout << "Barometer Temperature: " << values[0] << " Pressure: " << values[1] << "\n";
		delay((int)interval);
	}
}

void *runMagnetometer(void *interval)
{
	//Initializing the sensor
	Magnetometer *magnetometer = new Magnetometer();
	double values[3];

	//Infinite loop to keep measuring --> TODO: Need to be changed
	while (1)
	{
		magnetometer->getMagnetometerValues(values);
		cout << "Magnet x=" << values[0] << " y=" << values[1] << " z=" << values[2] << "\n";
		delay((int)interval);
	}
}

int main(void)
{
	cout << "Starting Flight Controller\n";
	
	//Creating the threads
	pthread_t t1, t2, t3, t4;

	//Starts the Thread with the threadId in arg[0], arg[2] is the method, which is called by the thread, arg[3] the arguments of the method
	//int thread1 = pthread_create(&t1, NULL, runUltrasonic, (void *)500);
	int thread2 = pthread_create(&t2, NULL, runGyroAccelerometer, (void *)0);
	//int thread3 = pthread_create(&t3, NULL, runBarometer, (void *)500);
	//int thread4 = pthread_create(&t4, NULL, runMagnetometer, (void *)500);

	//Checking if there was a problem with creating the Threads
	/*if (thread1 != 0)
	{
		cout << "Error while creating the Thread 1 for the Sensor. Error Code " << thread1;
		exit(1);
	}
	if (thread2 != 0)
	{ 
		cout << "Error while creating the Thread 2 for the Sensor. Error Code " << thread2;
		exit(1);
	}
	if (thread3 != 0)
	{
		cout << "Error while creating the Thread 3 for the Sensor. Error Code " << thread2;
		exit(1);
	}
	if (thread4 != 0)
	{
		cout << "Error while creating the Thread 4 for the Sensor. Error Code " << thread2;
		exit(1);
	}*/

	//Waits until the threads are interrupted
	pthread_join(t1, (void**)1);
	pthread_join(t2, (void**)1);
	pthread_join(t3, (void**)1);
	pthread_join(t4, (void**)1);
	
	cout << "Done!";
	return (0);
}