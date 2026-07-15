#ifndef TIMER_H
#define TIMER_H

class Timer{
    public:
        float time;
        float tarTime;
        bool running;
        Timer();
        Timer(float tarTime);
        void Start();
        bool TimeOut();
        void Update();
        void Reset();
};


#endif