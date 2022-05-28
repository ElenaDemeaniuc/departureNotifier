#include <Arduino.h>
#include "Notifier.h"

Notifier::Notifier()
{
}

void Notifier::SetTimer(int a)
{
    int b = a;
    timer_remain = timer_remain + b;
}

void Notifier::Notifier_loop()
{
    unsigned long currenttime = millis();
    if (currenttime == lasttime)
    {
        return;
    }
    lasttime = currenttime;
    timerCounter++;
    if (timerCounter == 1000)
    {
        timerCounter = 0;
        beepRequest = false;
        if (timer_remain < 0)
        {
            timer_remain = 0;
        }
        if (timer_remain != 0)
        {

            timer_remain = timer_remain - 1000;
            if (timer_remain == 0)
            {
                beepRequest = true;
            }
        }
    }
}

int Notifier::GetTimer()
{
    int f = timer_remain;
    return f;
}

bool Notifier::GetBeep()
{
    return beepRequest;
}
