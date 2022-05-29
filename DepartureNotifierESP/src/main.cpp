#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>
#include "Rotary.h"
#include "Display.h"
#include "Buzzer.h"
#include "Notifier.h"

// Pins defined for rotary encoder
#define Key D7 // GPIO13
#define S1 D6  // GPIO14
#define S2 D5  // GPIO12

//#define S3 D4 // GPIO2

// variables for rotary encoder
const int thresholdReset = 10000;
const int threshold = 1500;

unsigned long ElapsedTime;
unsigned long lasttime = 0;
unsigned long lasttime2 = 0;
// int32 TimeFromData;
bool StartDataLoop = false;

int btnTime = 0;
bool btn;
int rotarycounter = 0;
int pressTime = 0;
int timing = 0;

int index1 = 0;
int index2 = 0;
int index3 = 0;
bool Shortpress = false;

int NotifyTimer;
bool NotifyBuzzer1;
bool NotifyBuzzer2;

String CityArray[3] = {"Budapest", "Wien", "Erd"};

String startcity = "Start";
String endcity = "End";
String datastartplace = "Empty";
String datastarttime = "Empty";
String dataendplace = "Empty";
String dataendtime = "Empty";
String datatotaltime = "Empty";
String dataleavehome = "Empty";

String a = "Empty";
String b = "Empty";
String c = "Empty";
String d = "Empty";
String e = "Empty";
String f = "Empty";

Rotary Encoder1(S1, S2, Key);
Notifier Notify;

//

//

void TimerOnDisplay(int input)
{
  int timer = input;
  if (timer > 0 && timer < 60000 * 360)
  {
    int seconds = (timer / 1000) % 60;
    int minutes = ((timer / (1000 * 60)) % 60);
    int hours = ((timer / (1000 * 60 * 60)) % 24);
    Display_timer(hours, minutes, seconds);
  }
}

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
int datasize = 11; // example for testing

String big[11];
int TimeFromData = 38171000; // example for testing
//
//
//
//
String My_S = "11:50;40800000;Budapest-Deli;Erd felso;12:14;0:24 ";

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

  ElapsedTime = millis();
  if (ElapsedTime - lasttime > 200)
  {
    lasttime = ElapsedTime;
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
          Shortpress = true;
          Serial.println("Press is short");
        }
        else if (pressTime >= threshold && pressTime < thresholdReset)
        {
          // During timer setting this will be often occuring
          Serial.println("Press is medium");
        }
        else if (pressTime >= thresholdReset)
        {
          Display_Reset();
          startcity = "Start";
          endcity = "End";
          a = "Empty";
          b = "Empty";
          c = "Empty";
          d = "Empty";
          e = "Empty";
          f = "Empty";
          Serial.println("Press is long");
          Notify.ResetTimer();
        }
        pressTime = 0;
      }
      btnTime = 0;
    }

    if (rotarycounter != 0)
    {
      if (btn == 0)
      {
        index1 = index1 + rotarycounter;
        if (index1 < 0)
          index1 = 2;
        if (index1 > 2)
          index1 = 0; // thus we rotate in circle a-b-c-a-b-..
        index2 = index2 + rotarycounter;
        if (index2 < 0)
          index2 = 2;
        if (index2 > 2)
          index2 = 0;
        index3 = index3 + rotarycounter;
        if (index3 < 0)
          index3 = 0;
        Serial.print("Encoder rotation: ");
        Serial.println(index1);
        Serial.print("Encoder rotation: ");
        Serial.println(index2);
        Serial.print("Encoder rotation: ");
        Serial.println(index3); // upper limit - size of temporary array
      }
      else if (btn != 0)
      {
        timing = timing + rotarycounter * 1000;
        if (timing < 0)
          timing = 0;
        Serial.print("timing: ");
        Serial.println(timing);
      }
      rotarycounter = 0;
    }

    // Array functions
    if (startcity.equals("Start"))
    {
      Display_StartPoint(CityArray[index1]);
      if (Shortpress == true)
      {
        startcity = CityArray[index1];
        Display_StartPoint(startcity);
        Shortpress = false;
      }
    }

    if (!startcity.equals("Start") && endcity.equals("End"))
    {
      Display_EndPoint(CityArray[index2]);
      if (Shortpress == true)
      {
        if (!startcity.equals(CityArray[index2]))
        {
          endcity = CityArray[index2];
          Display_EndPoint(endcity);
          Shortpress = false;
          StartDataLoop = true;
        }
        else
        {
          Shortpress = false;
        }
      }
    }

    if (!startcity.equals("Start") && !endcity.equals("End") && StartDataLoop == true)
    {
      for (int i = 0; i < datasize; i++)
      {
        big[i] = data[i];
        big[i].remove(0, startcity.length() + 1);
        big[i].remove(0, endcity.length());
      }
      int index = My_S.indexOf(';');
      if (index != 0)
      {
        if (a.equals("Empty"))
        {
          a = My_S.substring(0, index);
          Serial.println(a);
        }
        else if (b.equals("Empty"))
        {
          b = My_S.substring(0, index);
          Serial.println(b);
          timing = b.toInt() - TimeFromData;
        }
        else if (c.equals("Empty"))
        {
          c = My_S.substring(0, index);
          Serial.println(c);
        }
        else if (d.equals("Empty"))
        {
          d = My_S.substring(0, index);
          Serial.println(d);
        }
        else if (e.equals("Empty"))
        {
          e = My_S.substring(0, index);
          Serial.println(e);
        }
        else if (f.equals("Empty"))
        {
          f = My_S;
          Serial.println(f);
          StartDataLoop = false;
        }
      }
      My_S.remove(0, index + 1);
      Display_StartTimeObject(a);
      Display_StartPoint(c);
      Display_EndPoint(d);
      Display_EndTime(e);
      Display_TimeOnRoad(f);
    }

    // at the end of first loop
    if (timing != 0)
    {
      Notify.SetTimer(timing);
      timing = 0;
    }
  }

  Notify.Notifier_loop();
  unsigned long time2 = millis();
  if (time2 - lasttime2 > 200)
  {
    lasttime2 = time2;
    NotifyTimer = Notify.GetTimer();
    TimerOnDisplay(NotifyTimer);
    NotifyBuzzer1 = Notify.GetBeep();
    if (NotifyBuzzer1 == true)
    {
      NotifyBuzzer2 == true;
      // first variable will be set to false very fast
    }
    // Buzzer_On(NotifyBuzzer2);
  }
}
