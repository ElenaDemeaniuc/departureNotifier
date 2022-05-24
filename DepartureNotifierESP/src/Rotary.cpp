#include <Arduino.h>
#include "Rotary.h"

Rotary::Rotary(int PinS1, int PinS2, int PinKey) 
{      
  S1=PinS1;
  S2=PinS2;
  Key=PinKey;
  Rotary_setup();
 };
    
    
void Rotary::Rotary_setup() {
  // put your setup code here, to run once: 
  pinMode(Key, INPUT); 
  pinMode(S1, INPUT);
  pinMode(S2, INPUT); 
  lastStateA=digitalRead(S1);
  lastStateC=digitalRead(S2);
  lastKey=digitalRead(Key);
}

void Rotary::Rotary_loop() {
  // main rotary code
 StateNowA=digitalRead(S1);
 if (StateNowA!=lastStateA) 
 {
   lastStateA=StateNowA;
   StateNowC=digitalRead(S2);
   if (StateNowC!=lastStateC) 
   {
      lastStateC=StateNowC;
      if (StateNowA==StateNowC)
      counter++;
      else
      counter--;
   }
  }
 KeyValue=!digitalRead(Key);

 if(IsButtonPressed){
  releaseTime=millis();
     PressButtonTime=releaseTime-pressTime;
 }

 if (KeyValue!=lastKey) 
  {
    if(KeyValue!=0 ){
     IsButtonPressed=true;
     pressTime=millis();
     PressButtonTime=0;
    }
    else{
     IsButtonPressed=false;
     }
    
    lastKey=KeyValue;   
  }
}

int Rotary::GetDeltaCounter()
{ 
  int DeltaCounter=counter;
  counter=0;
  return DeltaCounter;
}

bool Rotary::GetIsButtonPressed() 
{
  return IsButtonPressed;
}

int Rotary::GetPressButtonTime()
{ 
  int time= PressButtonTime;
  return time;  
}
