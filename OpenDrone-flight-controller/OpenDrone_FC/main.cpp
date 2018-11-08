#include <iostream>
#include "FlightController.h"
using namespace std;

int main(void)
{
	FlightController *fc = new FlightController();

	cout << "Starting Flight Controller\n";
	int status = fc->run();
	if (status == 0) 
	{
		cout << "Stopping Flight Controller!";
		return (0);
	}
	else
	{
		cout << "Stopping Flight Controller!";
		return (status);
	}
}