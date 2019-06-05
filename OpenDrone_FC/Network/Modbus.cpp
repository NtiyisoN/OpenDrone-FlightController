/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.1 14.02.2019
 */
#include <list> 
#include <iterator>
#include "Modbus.h";
#include "thread"
#include "TCPServer.h"
#include "Command.h"
using namespace std;

//bool boolRunThread = false;
//int timeoutCounter = 0;


/*static void getTimeout() {
	while (!boolRunThread) {
		delay(50);
	}

	while (boolRunThread) {
		PID *pid = PID::getInstanceCreated();
		if (pid != 0)
		{
			timeoutCounter++;
			cout << timeoutCounter << "\n";
			if (timeoutCounter >= 5) {
				//TODO: Land drone
				pid->interruptPid();
				boolRunThread = false;
			}
		}
		delay(1000);
	}
}*/

//thread timeout(getTimeout);

Modbus::Modbus()
{
}

Modbus::~Modbus()
{
}

void Modbus::checkMotors(PWMMotorTest *pwm, int motorNum) {
	pwm->ExitMotor();
	pwm->ArmMotor();
	motorNum -= 1;

	pwm->SetSpeed(motorNum, 1200);
	delay(2500);
	pwm->SetSpeed(motorNum, 0);
	pwm->ExitMotor();
}

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

    //if(result.size)
    //cout << "--------------------------------Packets:" << result.size();
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
			//Throttle
			if (functionCode == 1) { pid->setThrottle(stoi(data)); }
			//Yaw
			if (functionCode == 3) { pid->setYawSetpoint(stoi(data)); }
			//Pitch
			if (functionCode == 5) { pid->setPitchSetpoint(stoi(data)); }
			//Roll
			if (functionCode == 7) { pid->setRollSetpoint(stoi(data)); }
			//Interrupt PID/Motors
			if (functionCode == 10) { pid->interruptPid(); }
			//Calibrate
			if (functionCode == 20) { Calibration *c = new Calibration(pid->getOrientatin()); c->calibrate(); }
			//Arm Motor
			if (functionCode == 30) { pid->armMotor(); pid->setRun(true);}
			//PID - P
			if (functionCode == 333) { pid->setP(stof(data)); }
			//PID - I
			if (functionCode == 334) { pid->setI(stof(data)); }
			//PID - D
			if (functionCode == 335) { pid->setD(stof(data)); }
			//Test Motors
			if (functionCode == 17) { PWMMotorTest *pwm = pid->getPwmMotorTest(); checkMotors(pwm, stoi(data)); };
			//Height-Control on 
			if (functionCode == 47) {};
			//Height-Control off 
			if (functionCode == 48) {};

		}

        string parity = result.at(5+(i*3));
        string am = "Packages: "+ to_string(packages) + "SlaveID: " + to_string(slaveID) + "(1)" + ", FC: " + to_string(functionCode) + "("  +to_string(3+(i*3)) +")" + ", Data: " + data + "(" + to_string(4 + (i * 3)) + ")" + ", parity: " + parity + "(" + to_string(5 + (i * 3)) + ")";
        //cout << am << " / ";
    }
    //cout << endl;
    /*for (i = 0; i < result.size(); i++) {
        cout << result.at(i) << std::endl;
    }*/
    //cout.flush();
}


