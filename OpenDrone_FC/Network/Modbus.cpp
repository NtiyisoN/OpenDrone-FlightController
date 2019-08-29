/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to react on the commands received from the app
 *
 * 	@author Markus Kurzmann, Thomas Brych
 * 	@version 0.0.2 26.06.2019
 */
#include <list> 
#include <iterator>
#include "Modbus.h";
#include "thread"
#include "TCPServer.h"
#include "Command.h"
using namespace std;

Modbus::Modbus()
{
}

Modbus::~Modbus()
{
}

/**
	Method to check the spin direction of the motors
	@return void

	@params PWMMotorTest *pwm, int motorNum
*/
void Modbus::checkMotors(PWMMotorTest *pwm, int motorNum) {
	pwm->ExitMotor();
	pwm->ArmMotor();
	motorNum -= 1;

	pwm->SetSpeed(motorNum, 1150);
	delay(2500);
	pwm->SetSpeed(motorNum, 0);
	pwm->ExitMotor();
}

/**
	This methods interprets the received commands from the app
	@return void

	@params string str
*/
void Modbus::Interpret(string str)
{
	cout << str << "\n";
	cout.flush();
    std::stringstream ss(str);
    vector<string> result;
    while (ss.good()) 
    {
        string substr;
        getline(ss, substr, ';');
        result.push_back(substr);
    }

	if (result.size() <= 2) {
		return;
	}

    int i;
    cout << endl;
    int slaveID = stoi(result.at(1));

    //Amount of commands
    int packages = stoi(result.at(2));

	if (packages == 1) {
		int functionCode = stoi(result.at(3));
	}

    for (i = 0; i < packages; i++) {
        int functionCode = stoi(result.at(3+(i*3)));
        string data = result.at(4+(i*3));

		PID *pid = PID::getInstanceCreated();

		if (pid != 0) {
			switch (functionCode) {
				//Throttle
				case 1: { pid->setThrottle(stoi(data)); break; }
				//Yaw
				case 3: { pid->setYawSetpoint(stoi(data)); break; }
				//Pitch
				case 5: { pid->setPitchSetpoint(stoi(data)); break; }
				//Roll
				case 7: { pid->setRollSetpoint(stoi(data)); break; }
				//Interrupt PID/Motors
				case 10: { pid->interruptPid(); break; }
				//Calibrate
				case 20: { Calibration *c = new Calibration(pid->getOrientatin()); c->calibrate(); break; }
				//Arm Motor
				case 30: { pid->armMotor(); pid->setRun(true); break; }
				//PID - P
				case 333: { pid->setP(stof(data)); break; }
				//PID - I
				case 334: { pid->setI(stof(data)); break; }
				//PID - D
				case 335: { pid->setD(stof(data)); break; }
				//Test Motors
				case 17: { PWMMotorTest *pwm = pid->getPwmMotorTest(); checkMotors(pwm, stoi(data)); break; }
				//Height Control on
				case 47: { pid->updateHeightControl(); break; }
			}
		}
        string parity = result.at(5+(i*3));
        string am = "Packages: "+ to_string(packages) + "SlaveID: " + to_string(slaveID) + "(1)" + ", FC: " + to_string(functionCode) + "("  +to_string(3+(i*3)) +")" + ", Data: " + data + "(" + to_string(4 + (i * 3)) + ")" + ", parity: " + parity + "(" + to_string(5 + (i * 3)) + ")";
    }
}
