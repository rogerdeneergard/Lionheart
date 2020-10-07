#pragma once
#include<string>
class Time {
public:
	Time(unsigned long int time = 0, unsigned int increment = 1) : time(time), increment(increment) {}

	void TimeIncrement() { time += increment; }

	void SetTime(unsigned long int _time) { time = _time; }
	
	unsigned long int GetTime() { return time; }

	void SetIncrement(unsigned int _increment) { increment = _increment; }

	unsigned int GetIncrement() { return increment; }

private:
	unsigned long int time;
	unsigned int increment;
};