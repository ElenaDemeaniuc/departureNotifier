#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BusIO_Register.h>
#include "Display.h"

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Display_setup()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.clearDisplay();
}

void Display_Start()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("0h");
    display.setCursor(36, 0);
    display.println("00m");
    display.setCursor(84, 0);
    display.println("00s");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1, 17);
    display.drawLine(0, 15, 127, 15, WHITE);
    display.drawLine(0, 37, 127, 37, WHITE);
    display.drawLine(0, 55, 127, 55, WHITE);
    display.drawRect(46, 25, 33, 11, WHITE);
    display.setCursor(1, 17);
    display.println("Start station");
    display.setCursor(48, 27);
    display.println("00:00");
    display.setCursor(1, 39);
    display.println("End Station");
    display.setCursor(48, 47);
    display.println("00:00");
    display.setCursor(1, 57);
    display.println("Time on road 00:00");

    display.display();
}

void Display_timer(int a, int b, int c)
{
    int x = 0;
    int y = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.fillRect(x, y, 10, 14, SSD1306_BLACK);
    display.println(a);
    display.setCursor(x + 36, y);
    display.fillRect(x + 36, y, 22, 14, SSD1306_BLACK);
    display.println(b);
    display.setCursor(x + 84, y);
    display.fillRect(x + 84, y, 22, 14, SSD1306_BLACK);
    display.println(c);
    display.display();
}

void Display_StartPoint(String input)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1, 17);
    display.fillRect(1, 17, 126, 7, SSD1306_BLACK);
    display.println(input);
    display.display();
}

void Display_EndPoint(String input)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1, 39);
    display.fillRect(1, 39, 126, 7, SSD1306_BLACK);
    display.println(input);
    display.display();
}

void Display_TimeOnRoad(String input)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(82, 57);
    display.fillRect(82, 57, 35, 7, SSD1306_BLACK);
    display.println(input);
    display.display();
}

void Display_EndTime(String input)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(48, 47);
    display.fillRect(48, 47, 35, 7, SSD1306_BLACK);
    display.println(input);
    display.display();
}

void Display_StartTimeObject(String input)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(48, 27);
    display.fillRect(48, 27, 35, 7, SSD1306_BLACK);
    display.println(input);
    display.display();
}

void Display_StartTimeSelect(String input1, String input2, String input3)
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2, 27);
    display.fillRect(2, 27, 35, 7, SSD1306_BLACK);
    display.println(input1);
    display.setCursor(48, 27);
    display.fillRect(48, 27, 35, 7, SSD1306_BLACK);
    display.println(input2);
    display.setCursor(98, 27);
    display.fillRect(98, 27, 35, 7, SSD1306_BLACK);
    display.println(input3);
    display.display();
}

void Display_Reset()
{
    int x = 0;
    int y = 0;
    display.fillRect(x, y, 10, 14, SSD1306_BLACK);
    display.fillRect(x + 36, y, 22, 14, SSD1306_BLACK);
    display.fillRect(x + 84, y, 22, 14, SSD1306_BLACK);
    display.fillRect(1, 17, 126, 7, SSD1306_BLACK);
    display.fillRect(1, 39, 126, 7, SSD1306_BLACK);
    display.fillRect(82, 57, 35, 7, SSD1306_BLACK);
    display.fillRect(48, 47, 35, 7, SSD1306_BLACK);
    display.fillRect(2, 27, 35, 7, SSD1306_BLACK);
    display.fillRect(48, 27, 35, 7, SSD1306_BLACK);
    display.fillRect(98, 27, 35, 7, SSD1306_BLACK);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("0");
    display.setCursor(36, 0);
    display.println("00");
    display.setCursor(84, 0);
    display.println("00");
    display.display();
}