#include "oled.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void myoled::starten()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
    }
     clrDisplay();
}

void myoled::clrDisplay(){
     display.clearDisplay();
     display.display();
}

void myoled::oledDisplayCenter(String text, uint16_t setHeight)
{
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;

    display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

    if (setHeight <= 0)
    {
        display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
    }
    else
    {
        display.setCursor((SCREEN_WIDTH - width) / 2, setHeight);
    }

    display.println(text);
}

void myoled::setText(String txt, byte x, byte y)
{
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.println(txt);
    display.display();
}