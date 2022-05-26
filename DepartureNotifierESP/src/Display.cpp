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
    display.setCursor(2, 2);
    display.println("3h 55m 47s");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(1, 17);
    display.println("Budapest Kelenfold");
    display.setCursor(1, 27);
    display.println("16:40");
    display.setCursor(1, 37);
    display.println("Erd Felso");
    display.setCursor(1, 47);
    display.println("17:20");
    display.setCursor(1, 57);
    display.println("Time on road 00:24");
    display.display();
}