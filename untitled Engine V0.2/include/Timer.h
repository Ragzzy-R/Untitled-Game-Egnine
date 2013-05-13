#pragma once

#include <sstream>
class Timer
{
public:
	Timer(void);
	long startTime;
	int currentTime;
	bool UEDelay(long);
	~Timer(void);
};

