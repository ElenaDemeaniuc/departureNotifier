#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>
#include "Rotary.h"
#include "Display.h"

// Pins defined for rotary encoder
#define Key D7 // GPIO13
#define S1 D6  // GPIO14
#define S2 D5  // GPIO12

// variables for rotary encoder
const int threshold = 5000;
int btnTime = 0;
int pressTime = 0;
bool btn;
int rotarycounter = 0;
unsigned long lasttime = 0;
int RotationNoPress = 0;
int RotationPress = 0;

Rotary Encoder1(S1, S2, Key);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Display_setup();
  Display_Start();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Encoder1.Rotary_loop();

  unsigned long time = millis();
  if (time - lasttime > 200)
  {
    lasttime = time;
    rotarycounter += Encoder1.GetDeltaCounter();
    btn = Encoder1.GetIsButtonPressed();
    btnTime = Encoder1.GetPressButtonTime();

    if (rotarycounter == 0)
    {
      if (btn != 0)
      {
        pressTime = pressTime + btnTime;
      }
      else if (btn == 0 && pressTime != 0)
      {
        if (pressTime < threshold)
        {
          Serial.println("Press is short");
          // Short press does noting because it appear quite often during Timer setting
        }
        else if (pressTime >= threshold)
        {
          Serial.println("Press is long");

          // A long press of approximately 3 seconds would be changing stove number
        }
        pressTime = 0;
      }
      btnTime = 0;
    }

    if (rotarycounter != 0)
    {
      if (btn == 0)
      {
        RotationNoPress = RotationNoPress + rotarycounter;
        if (RotationNoPress < 0)
          RotationNoPress = 0;
        if (RotationNoPress > 10)
          RotationNoPress = 10;
        Serial.print("Encoder rotation: ");
        Serial.println(RotationNoPress);
      }
      else if (btn != 0)
      {
        RotationPress = RotationPress + rotarycounter;
        if (RotationPress < 0)
          RotationPress = 0;
        Serial.print("Encoder rotation with pushbutton: ");
        Serial.println(RotationPress);
      }
      rotarycounter = 0;
    }
  }
}