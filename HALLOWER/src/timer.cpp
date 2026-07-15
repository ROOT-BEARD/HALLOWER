#include "Timer.h"
#include "raylib.h"

//Default constructor
Timer::Timer(){
    time = 0.0f;
    tarTime = 0.0f;
    running = false;
}

Timer::Timer(float tarTime){
    time = 0.0f;
    this->tarTime = tarTime;
}

void Timer::Start(){
    running = true;
}

void Timer::Reset(){
    running = false;
    time = 0.0f;
}

bool Timer::TimeOut(){
    bool complete = false;
    if(time >= tarTime){    
        complete = true;
        time = 0.0;
        running = false;
    }
    return complete;
}

void Timer::Update(){
    if(running) time += GetFrameTime();
}