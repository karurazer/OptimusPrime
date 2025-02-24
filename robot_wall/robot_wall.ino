#include "motor_control.h"


void setup()
{
  Serial.begin(9600);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);

  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);

  pinMode(SR1, INPUT);
  pinMode(SR2, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(SR1), countRotationsSr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SR2), countRotationsSr2, FALLING);
  all_s();
}
int speed = 250;
void optimus(){
  if (stopDistance(12)){
    setLeftMotor(-speed);
    setRightMotor(-speed);
  } else if(moreDistance(20)){
    setBothMotor(speed);
  }
  
}
void loop()
{
  left_f(250);
  stopAfterLeft(0.75);
  delay(500);

}



