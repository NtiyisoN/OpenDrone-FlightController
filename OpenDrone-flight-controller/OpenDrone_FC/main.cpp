#include "UltraSonic.h"
#include "Barometer.h"
#include <iostream>
#include <pthread.h>
#include <wiringPi.h>
#include "GY87.h"

using namespace std;

void *runUltrasonic(void *interval)
{
	//Initializing the wiringPi-Gpio's
	int rc = wiringPiSetupGpio();
	if (rc != 0) {
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	//Initialize the sensors
	int len = 2;
	UltraSonic* sensors[len];
	sensors[0] = new UltraSonic(17, 27, 1);
	sensors[1] = new UltraSonic(17, 27, 2);

	//Infinite loop to keep measuring the distance --> TODO: Need to be changed
	double curDistance;
	while (1) {
		for (int i = 0; i < len; i++) {
			curDistance = sensors[i]->distance();
			cout << sensors[i]->getId() << " Distance: " << curDistance << "\n";
			delay(2);
		}
		delay((int)interval);
	}
}

void *run10DOF(void *interval)
{
	GY87* gy = new GY87();
}

void *runBarometer(void *interval)
{
	Barometer *barometer = new Barometer();

	double *ar;
	barometer->getBarometerValues(ar);
	cout << ar[0] << " " << ar[1];

}

int main(void)
{
	cout << "Starting Flight Controller\n";
	
	//Creating the threads
	pthread_t t1, t2, t3;

	//Starts the Thread with the threadId in arg[0], arg[2] is the method, which is called by the thread
	/*int thread1 = pthread_create(&t1, NULL, runUltrasonic, (void *)0);
	int thread2 = pthread_create(&t2, NULL, run10DOF, (void *)0);*/
	int thread1 = 0, thread2 = 0;
	int thread3 = pthread_create(&t3, NULL, runBarometer, (void *)0);

	//Checking if there was a problem with creating the Threads
	if (thread1 != 0)
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
		cout << "Error while creating the Thread 2 for the Sensor. Error Code " << thread2;
		exit(1);
	}

	//Waits until the threads are interrupted
	pthread_join(t1, (void**)1);
	pthread_join(t2, (void**)1);
	pthread_join(t3, (void**)1);
	
	cout << "Done!";
	return (0);
}