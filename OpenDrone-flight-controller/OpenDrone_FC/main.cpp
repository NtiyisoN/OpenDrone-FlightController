#include <iostream>
#include <fstream>
#include <pthread.h>
#include <wiringPi.h>
#include "PWMMotorTest.h"
#include "FlightController.h"

using namespace std;

int main(void)
{
	cout << "Starting Flight Controller\n";
	
	//Creating the threads
	//pthread_t t1, t2, t3, t4;

	//Starts the Thread with the threadId in arg[0], arg[2] is the method, which is called by the thread, arg[3] the arguments of the method
	//int thread1 = pthread_create(&t1, NULL, runUltrasonic, (void *)500);
	//int thread2 = pthread_create(&t2, NULL, runGyroAccelerometer, (void *)0);
	//int thread3 = pthread_create(&t3, NULL, runBarometer, (void *)500);
	//int thread4 = pthread_create(&t4, NULL, runMagnetometer, (void *)500);

	//Checking if there was a problem with creating the Threads
	/*if (thread1 != 0)
=======
	FlightController *fc = new FlightController();

	cout << "Starting Flight Controller\n";
	int status = fc->run();
	if (status == 0) 
>>>>>>> 273ecb5f711d6dc25ace7a1dd7571ede8599dc3a
	{
		cout << "Stopping Flight Controller!";
		return (0);
	}
	else
	{
		cout << "Stopping Flight Controller!";
		return (status);
	}
<<<<<<< HEAD
	if (thread4 != 0)
	{
		cout << "Error while creating the Thread 4 for the Sensor. Error Code " << thread2;
		exit(1);
	}*/

	//Waits until the threads are interrupted
	/*pthread_join(t1, (void**)1);
	pthread_join(t2, (void**)1);
	pthread_join(t3, (void**)1);
	pthread_join(t4, (void**)1);*/

	int rc = wiringPiSetupGpio();
	if (rc != 0)
	{
		cout << "Failed to wiringPiSetupGpio()\n";
		exit(1);
	}

	PWMMotorTest* pw = new PWMMotorTest();
	pw->StartMotors(); //Start Motors !!ONLY one time!!
	pw->SetSpeed(210); //Sets speed
	delay(5000);
	pw->SetSpeed(300);
	delay(5000);
	pw->SetSpeed(400);
	delay(5000);
	pw->SetSpeed(180);
	delay(5000);
	pw->SetSpeed(0);
	pw->SetSpeed(205);
	delay(5000);
	pw->SetSpeed(180);
	delay(5000);
	pw->SetSpeed(0);

	
	cout << "Done!";
	return (0);
}