#pragma once

#include <memory>

class Time
{
public:
	static Time* GetInstance();
	
private:
	static std::shared_ptr<Time> instance;

	unsigned int frameCount;
	double beginTime;
	double lastTime;
public:
	double deltaTime;
	void BeginTime();
	void NewFrameTime();

	Time() {};
};

