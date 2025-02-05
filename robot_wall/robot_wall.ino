#include "motor_control.h"


void setup()
{
  Serial.begin(9600);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(SR1, INPUT);
  pinMode(SR2, INPUT);

  attachInterrupt(digitalPinToInterrupt(SR1), countRotationsSr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SR2), countRotationsSr2, FALLING);
}
void loop()
{
  read_sensors();
  setBothMotor(-255);
}



