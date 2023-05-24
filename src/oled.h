#ifndef oled_h
#define oled_h

#include <Arduino.h>


class myoled
{
private:

    
public:
    void starten();
    void startDaten(String row1, String row2, String row3, String row4);
    void setCurrentSensorValues(int val, String sensor, int zielval, byte row);
    void setText(String txt, byte x, byte y);
    void oledDisplayCenter(String text, uint16_t setHeight);
    void clrDisplay();
};

#endif