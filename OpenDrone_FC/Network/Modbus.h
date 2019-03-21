/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Markus Kurzmann
 * 	@version 0.0.1 14.02.2019
 */
#include "iostream";
#include <string.h>;
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
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
};

