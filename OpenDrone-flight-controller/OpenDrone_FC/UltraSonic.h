#pragma once
class UltraSonic
{
public:
	UltraSonic();
	UltraSonic(int pin_trigger, int pin_echo, int num);
	~UltraSonic();
	double distance();
	int getId();

private:
	int pin_trigger;
	int pin_echo;
	int id;
	int read_word(int reg);
	int read_word_2c(int reg);
};


