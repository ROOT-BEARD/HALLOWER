#include "Timer.h"
#include "raylib.h"

// Default constructor
Timer::Timer()
{
    time = 0.0f;
    tarTime = 0.0f;
    running = false;
}

Timer::Timer(float tarTime)
{
    time = 0.0f;
    this->tarTime = tarTime;
    running = false;
}

void Timer::Start()
{
    time = 0.0f;
    running = true;
}

void Timer::Reset()
{
    running = false;
    time = 0.0f;
}

bool Timer::TimeOut()
{
    if(!running) return false;
    return time >= tarTime;
}

void Timer::Update()
{
    if (running)
        time += GetFrameTime();
        if(time >= tarTime) time = tarTime;
}