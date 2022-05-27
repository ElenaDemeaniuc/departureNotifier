#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <Arduino.h>

class Buzzer
{
private:
    int S3;
    int beepsRemain = 0;
    const int beeptime = 1000;
    bool Buzzer_Output = false;
    int buzzCounter = 0;
    unsigned long last_time = 0;
    void Buzzer_setup();

public:
    Buzzer(int Pin1);
    void Buzzer_On(bool a);
    bool GetBuzz();
};

#endif