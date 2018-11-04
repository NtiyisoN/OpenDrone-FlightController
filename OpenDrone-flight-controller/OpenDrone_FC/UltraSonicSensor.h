#pragma once
class UltraSonicSensor
{
public:
	UltraSonicSensor();
	UltraSonicSensor(int pin_trigger, int pin_echo, int num);
	~UltraSonicSensor();
	double distance();
	int getId();

private:
	int pin_trigger;
	int pin_echo;
	int id;
	int read_word(int reg);
	int read_word_2c(int reg);
};


