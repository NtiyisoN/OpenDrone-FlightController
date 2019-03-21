/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.1 14.02.2019
 */
#include "Modbus.h";
using namespace std;

Modbus::Modbus()
{
}

Modbus::~Modbus()
{
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

		PID *pid = PID::getInstance(NULL, NULL);

		if (pid->isInit()) {
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
			if (functionCode == 30) { pid->armMotor(); pid->setRun(true); }
			//PID - P
			if (functionCode == 333) { pid->setP(stof(data)); }
			//PID - I
			if (functionCode == 334) { pid->setI(stof(data)); }
			//PID - D
			if (functionCode == 335) { pid->setD(stof(data)); }
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
