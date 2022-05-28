#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include <Arduino.h>

class Notifier
{
private:
    int timerCounter = 0;
    int timer_remain = 0;
    unsigned long lasttime = 0;
    bool beepRequest;

public:
    Notifier();
    void Notifier_loop();
    void SetTimer(int t);
    int GetTimer();
    bool GetBeep();
};

#endif