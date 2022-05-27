#include <Arduino.h>
#include "Buzzer.h"

//  //if use pin for buzzer
Buzzer::Buzzer(int Pin1)
{
    S3 = Pin1;
    Buzzer_setup();
}

void Buzzer::Buzzer_setup()
{
    pinMode(S3, OUTPUT);
}

void Buzzer::Buzzer_On(bool a)
{
    bool buzz = a;
    if (buzz == true)
    {
        if (beepsRemain < 3)
        {
            unsigned long currenttime = millis();
            if (currenttime == last_time)
            {
                return;
            }
            last_time = currenttime;
            buzzCounter++;
            if (buzzCounter < beeptime)
            {
                Buzzer_Output = true;
            }
            Serial.println("Buzzer is ON\r\n");
            if (buzzCounter > beeptime && buzzCounter < beeptime * 2)
            {
                Buzzer_Output = false;
            }
            Serial.println("Buzzer is OFF\r\n");
            buzzCounter = 0;
            beepsRemain++;
        }
        beepsRemain = 0;
        Buzzer_Output = false;
    }
}

bool Buzzer::GetBuzz()
{
    return Buzzer_Output;
}