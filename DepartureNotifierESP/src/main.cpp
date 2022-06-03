#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "Rotary.h"
#include "Display.h"
#include "Buzzer.h"
#include "Notifier.h"
#include <stdio.h>

// Pins defined for rotary encoder
#define Key D7 // GPIO13
#define S1 D6  // GPIO14
#define S2 D5  // GPIO12

//#define S3 D4 // GPIO2

struct Train
{
  char citystart[10];
  char cityend[10];
  char timestart[6];
  int32 timeleave; // int timetoleave=atoi(train.timeleave)
  char stationstart[20];
  char stationend[20];
  char timeend[6];
  char timetotal[6];
};

struct Train *trains = NULL;
int CountTCP = 0;
int SizeArray = 0;
int TimeFromData = 0;

int indextrip[20];
int temp = 0;
int temp2 = 0;

// variables for rotary encoder
const int thresholdReset = 10000;
const int threshold = 1500;
unsigned long ElapsedTime;
unsigned long lasttime = 0;
unsigned long lasttime2 = 0;
bool StartDataLoop = false;
bool StartDisplayLoop = false;

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

Rotary Encoder1(S1, S2, Key);
Notifier Notify;

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

const char *ssid = "UPC5690742";
const char *password = "SWxfct3szwrf";
WiFiServer server(5000);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Display_setup();
  Display_Start();
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the TCP server
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();
  // // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        if (CountTCP == 0)
        {
          SizeArray = line.toInt();
          CountTCP++;
          if (trains != NULL)
          {
            free(trains);
          }
          trains = (struct Train *)malloc(SizeArray * sizeof(struct Train));
          continue;
        }
        else if (CountTCP == 1)
        {
          TimeFromData = line.toInt();
          CountTCP++;
          continue;
        }
        else if (CountTCP < SizeArray + 2)
        {
          strcpy(trains[CountTCP - 2].citystart, line.substring(0, line.indexOf(';')).c_str());
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].cityend, line.substring(0, line.indexOf(';')).c_str());
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].timestart, line.substring(0, line.indexOf(';')).c_str());
          line.remove(0, line.indexOf(';') + 1);

          trains[CountTCP - 2].timeleave = line.substring(0, line.indexOf(';')).toInt();
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].stationstart, line.substring(0, line.indexOf(';')).c_str());
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].stationend, line.substring(0, line.indexOf(';')).c_str());
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].timeend, line.substring(0, 5).c_str());
          line.remove(0, line.indexOf(';') + 1);

          strcpy(trains[CountTCP - 2].timetotal, line.c_str());
          line.remove(0, line.indexOf('\n') + 1);

          CountTCP++;
          continue;
        }
      }
    }
    client.stop();
    Serial.println("[Client disonnected]");
  }

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
          Serial.println("Press is long");
          temp2 = 0;
          StartDisplayLoop = false;
          Notify.ResetTimer();
          int index1 = 0;
          int index2 = 0;
          int index3 = 0;
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
        if (index3 > 19)
          index3 = 19;
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
      for (temp = 0; temp < SizeArray; temp++)
      {
        Serial.println(String(trains[temp].citystart));
        if (String(trains[temp].citystart).endsWith(startcity) && String(trains[temp].cityend).equals(endcity))
        {
          Serial.println("Meow");
          indextrip[temp2] = temp;
          temp2++;
        }
      }
      for (int f = temp2; f < 20; f++)
      {
        indextrip[f] = 1;
      }

      for (int t = 0; t < 20; t++)
      {
        Serial.println(indextrip[t]);
      }
      StartDisplayLoop = true;
      StartDataLoop = false;
    }

    if (StartDisplayLoop == true)
    {
      Display_StartTimeSelect(String(trains[indextrip[index3] - 1].timestart), String(trains[indextrip[index3]].timestart), String(trains[indextrip[index3] + 1].timestart));
      if (Shortpress == true)
      {
        Display_StartPoint(String(trains[indextrip[index3]].stationstart));
        Display_EndPoint(String(trains[indextrip[index3]].stationend));
        Display_EndTime(String(trains[indextrip[index3]].timeend));
        Display_TimeOnRoad(String(trains[indextrip[index3]].timetotal));
        Display_StartTimeObject(String(trains[indextrip[index3]].timestart));
        int timetemp = trains[indextrip[index3]].timeleave - TimeFromData;
        timing = timetemp;
        Shortpress = false;
        StartDisplayLoop = false;
      }
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
