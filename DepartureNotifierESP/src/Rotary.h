#ifndef _ROTARY_H_
#define _ROTARY_H_

#include <Arduino.h>

class Rotary
{
 private:
  int S1, S2, Key;
  int counter;
  int StateNowA;
  int lastStateA;
  int StateNowC;
  int lastStateC;
  int KeyValue;
  int lastKey; 
  int pressTime;
  int releaseTime;
  int PressButtonTime;
  bool IsButtonPressed;
  void Rotary_setup();
  
 public:
   Rotary(int PinS1, int PinS2, int PinKey); 
   void Rotary_loop();
   int GetDeltaCounter();
   int GetPressButtonTime();
   bool GetIsButtonPressed();
};
#endif
