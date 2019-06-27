/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to react on the commands received from the app
 *
 * 	@author Markus Kurzmann, Thomas Brych
 * 	@version 0.0.2 26.06.2019
 */
#include "iostream";
#include <string.h>;
#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sstream>
#include "../Controller/PID.h"
#include "../Controller/Calibration.h"

#pragma once

using namespace std;

class Modbus
{
public:
    Modbus();
    ~Modbus();
    void Interpret(string str);
	void checkMotors(PWMMotorTest *pwm, int motorNum);
};

