#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>
#include "Rotary.h"
#include "Display.h"
#include "Buzzer.h"

// Pins defined for rotary encoder
#define Key D7 // GPIO13
#define S1 D6  // GPIO14
#define S2 D5  // GPIO12

//#define S3 D4 // GPIO2

// variables for rotary encoder
const int threshold = 5000;
int btnTime = 0;
int pressTime = 0;
bool btn;
int rotarycounter = 0;
unsigned long lasttime = 0;
int RotationNoPress = 0;
int RotationPress = 0;

bool testBuzzer = false;

Rotary Encoder1(S1, S2, Key);
// Buzzer Buzzer1(S3);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Display_setup();
  Display_Start();
}

void loop()
{ // go through array with city names, do display_StartCity(array[i] )in circle, use rotary

  String data[] = {"Budapest;Erd;11:50;40800000;Budapest-Deli;Erd felso;12:14;0:24",
                   "Budapest;Erd;12:05;41700000;Budapest-Deli;Erd also;12:22;0:17",
                   "Budapest;Erd;12:10;42000000;Budapest-Deli;Erd also;12:26;0:16",
                   "Budapest;Erd;11:57;41520000;Kobanya-Kispest;Erd also;12:34;0:14",
                   "Erd;Budapest;14:29;51240000;Erd also;Budapest-Deli;14:49;0:20",
                   "Erd;Budapest;14:42;51720000;Erd felso;Budapest-Deli;15:09;0:27",
                   "Erd;Budapest;14:54;52440000;Erd felso;Kobanya-Kispest;15:31;0:37",
                   "Erd;Budapest;15:20;54300000;Erd also;Kobanya-Kispest;16:01;0:41",
                   "Wien;Budapest;12:42;44820000;Wien Hbf;Budapest-Keleti;15:19;2:37",
                   "Wien;Budapest;13:25;46200000;Wien Meidling;Budapest-Keleti;16:19;2:39",
                   "Wien;Budapest;14:37;51720000;Wien Hbf;Budapest-Keleti;17:19;2:42"};

  String startcity = "Budapest";
  String endcity = "Wien";

  String sub1 = "Budapest-Deli";
  String sub2 = "Erd felso";
  String sub3 = "11:50";
  String sub4 = "12:14";
  String sub5 = "0:24";
  int time1 = 45600000;
  int timenow = 42171000;
  int timer = time1 - timenow;
  int seconds = (timer / 1000) % 60;
  int minutes = ((timer / (1000 * 60)) % 60);
  int hours = ((timer / (1000 * 60 * 60)) % 24);

  // if long press, Display_Reset()

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
          // Serial.println("Press is long");
          Display_timer(hours, minutes, seconds);
          Display_StartPoint(sub1);
          Display_EndPoint(sub2);
          Display_TimeOnRoad(sub5);
          Display_EndTime(sub4);
          Display_StartTimeObject(sub3);
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

  // string a is "3940000", use a.toInt()
  //  int seconds = (int) (milliseconds / 1000) % 60 ;
  //  int minutes = (int)((milliseconds / (1000 * 60)) % 60);
  //  int hours = (int)((milliseconds / (1000 * 60 * 60)) % 24);
}