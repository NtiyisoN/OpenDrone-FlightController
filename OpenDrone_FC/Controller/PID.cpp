#include "PID.h"


PID::PID(Orientation *o, PWMMotorTest *p)
{
	orientation = o;
	pwm = p;
}

void PID::calcPid() {
	double *ar = orientation->getPitchRoll();
	/*double ar[3];
	ar[0] = 50;
	ar[1] = 0;
	ar[2] = 0;*/

	//Roll calculations
	pid_error_temp = ar[1] - pid_roll_setpoint;
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	if (pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
	else if (pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

	pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
	if (pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
	else if (pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

	pid_last_roll_d_error = pid_error_temp;

	//Pitch calculations
	pid_error_temp = ar[0] - pid_pitch_setpoint;
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	if (pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
	else if (pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;
	
	pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
	if (pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
	else if (pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

	pid_last_pitch_d_error = pid_error_temp;

	//Yaw calculations
	pid_error_temp = ar[2] - pid_yaw_setpoint;
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
	if (pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
	else if (pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

	pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
	if (pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
	else if (pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

	pid_last_yaw_d_error = pid_error_temp;

	if (pid_output_pitch < 0) pid_output_pitch *= -1;
	if (pid_output_roll < 0) pid_output_roll *= -1;
	if (pid_output_yaw < 0) pid_output_yaw *= -1;

	std::cout << pid_output_pitch << " " << pid_output_roll << " " << pid_output_yaw << "\n";
	std::cout.flush();
}

void PID::calcValues()
{
	std::cout << "Start";
	time = millis();
	while ((millis() - time) < 60000) {
		//The PID set point in degr1ees per second is determined by the roll receiver input.
		//In the case of deviding by 3 the max roll rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
		pid_roll_setpoint = 0;
		//We need a little dead band of 16us for better results.
		/*if (receiver_input_channel_1 > 1508)pid_roll_setpoint = receiver_input_channel_1 - 1508;
		else if (receiver_input_channel_1 < 1492)pid_roll_setpoint = receiver_input_channel_1 - 1492;

		pid_roll_setpoint -= roll_level_adjust;                                   //Subtract the angle correction from the standardized receiver roll input value.
		pid_roll_setpoint /= 3.0;  */                                               //Divide the setpoint for the PID roll controller by 3 to get angles in degrees.


		//The PID set point in degrees per second is determined by the pitch receiver input.
		//In the case of deviding by 3 the max pitch rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
		pid_pitch_setpoint = 0;
		/*//We need a little dead band of 16us for better results.
		if (receiver_input_channel_2 > 1508)pid_pitch_setpoint = receiver_input_channel_2 - 1508;
		else if (receiver_input_channel_2 < 1492)pid_pitch_setpoint = receiver_input_channel_2 - 1492;

		pid_pitch_setpoint -= pitch_level_adjust;                                  //Subtract the angle correction from the standardized receiver pitch input value.
		pid_pitch_setpoint /= 3.0;   */                                              //Divide the setpoint for the PID pitch controller by 3 to get angles in degrees.

		//The PID set point in degrees per second is determined by the yaw receiver input.
		//In the case of deviding by 3 the max yaw rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
		pid_yaw_setpoint = 0;
		//We need a little dead band of 16us for better results.
		/*if (receiver_input_channel_3 > 1050) { //Do not yaw when turning off the motors.
			if (receiver_input_channel_4 > 1508)pid_yaw_setpoint = (receiver_input_channel_4 - 1508) / 3.0;
			else if (receiver_input_channel_4 < 1492)pid_yaw_setpoint = (receiver_input_channel_4 - 1492) / 3.0;
		}*/

		calcPid();

		throttle = 300;                                      //We need the throttle signal as a base signal.

		esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 1 (front-right - CCW)
		esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 2 (rear-right - CW)
		esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw; //Calculate the pulse for esc 3 (rear-left - CCW)
		esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw; //Calculate the pulse for esc 4 (front-left - CW)

		if (esc_1 < 210) esc_1 = 210;                                         //Keep the motors running.
		if (esc_2 < 210) esc_2 = 210;                                         //Keep the motors running.
		if (esc_3 < 210) esc_3 = 210;                                         //Keep the motors running.
		if (esc_4 < 210) esc_4 = 210;                                         //Keep the motors running.

		if (esc_1 > 400) esc_1 = 400;                                           //Limit the esc-1 pulse to 400.
		if (esc_2 > 400) esc_2 = 400;                                           //Limit the esc-2 pulse to 400.
		if (esc_3 > 400) esc_3 = 400;                                           //Limit the esc-3 pulse to 400.
		if (esc_4 > 400) esc_4 = 400;                                           //Limit the esc-4 pulse to 400.  
		
																			   
		/*pwm->SetSpeed(0, esc_1);
		pwm->SetSpeed(1, esc_2);*/
		//pwm->SetSpeed(2, esc_1);
		//pwm->SetSpeed(3, esc_4);

		std::cout << esc_1 << " " << esc_2 << " " << esc_3 << " " << esc_4 << "\n";
		delay(10);
	}
	std::cout << "Stop";
	std::cout.flush();
	//pwm->SetSpeed(16, 0);
}


PID::~PID()
{
}
