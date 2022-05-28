#ifndef _DISPLAY_H
#define _DISPLAY_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>

void Display_setup();
void Display_Start();
void Display_timer(int a, int b, int c);
void Display_StartPoint(String input);
void Display_EndPoint(String input);
void Display_TimeOnRoad(String input);
void Display_EndTime(String input);
void Display_StartTimeSelect(String input1, String input2, String input3);
void Display_StartTimeObject(String input);
void Display_Reset();

#endif
