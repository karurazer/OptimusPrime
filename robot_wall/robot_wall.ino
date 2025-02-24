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
  if (stopDistance(18)){
    all_s();
    delay(30);
    left_f(250);
    stopAfterLeft(0.75);
    forward(250);
    delay(500);
    all_s();

    right_f(250);
    stopAfterRight(0.75);
    forward(250);
    delay(1000);
    all_s();
    right_f(250);
    stopAfterRight(0.75);
    forward(250);
    delay(500);
    all_s();
    left_f(250);
    stopAfterLeft(0.75);

  } else if(moreDistance(20)){
    setBothMotor(speed);
  }
}
void otimus_move(){
  forward(250);
  stopAfterRight(5);
  all_s();
  delay(1000);
  right_f(250);
  stopAfterRight(0.75);
  delay(500);
  left_f(250);
  stopAfterLeft(0.75);
  delay(10000);
}

void loop()
{
  otimus_move();

}



