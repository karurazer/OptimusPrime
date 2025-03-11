#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Servo.h>


// PINS
#define SERVO 4

#define NUM_SENSORS 8  
int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensorValues[NUM_SENSORS];
bool whereIsLine[NUM_SENSORS];  
bool prev[NUM_SENSORS];
int black = 0;


#define MA1 10 // left 
#define MA2 11 // left

#define MB1 6 // right
#define MB2 5 // right

#define SR1 0 // left wheel disconnect right now
#define SR2 3 // right wheel

// sonar pins
#define TRIG 9
#define ECHO 8

#define TRIGL 12
#define ECHOL 13

#define TRIGR 7
#define ECHOR 2


#define RIGHT_PRO_TURN_TIME 50
#define INCREASE_ON_RIGHT 20
#define START_VALUE_RIGHT 10;

#define LEFT_PRO_TURN_TIME 100
#define INCREASE_ON_LEFT 60
#define START_VALUE_LEFT 40

#define KEEPDISTANCE 6
#define FRONTDISTTURN 6

#define MINSPEED 170

// motor values
const int pulses = 20;
int rotationsSr1, rotationsSr2 = 0;
int diameter = 6.5;

float front;
float left;
float right;


// read sensor values
const int sensorDataInterval = 500;
int timerSensor, timerMoreDistance, timerGoodDistance, timerColor = 0;

// other values
Servo myservo; // servo value
bool withBotle = false;

void countRotationsSr1() 
{
  rotationsSr1++; 
}

void countRotationsSr2() 
{
  rotationsSr2++; 
}


void rightS()
{
  digitalWrite(MB1, HIGH);
  digitalWrite(MB2, HIGH);
}

void leftS()
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, HIGH);
}

void allS()
{
  leftS();
  rightS();
}

void leftF(int speed)
{
  analogWrite(MA2, speed);
  analogWrite(MA1, 0);
}

void rightF(int speed)
{
  analogWrite(MB1, speed);
  analogWrite(MB2, 0);
}

void rightB(int speed)
{
  analogWrite(MB2, speed);
  analogWrite(MB1, 0);
}

void leftB(int speed)
{
  analogWrite(MA1, speed);
  analogWrite(MA2, 0);
}

void forward(int speed)
{
  rightF(speed);
  leftF(speed);
}

void back(int speed)
{
  rightB(speed);
  leftB(speed);
} 

void setRightMotor(int speed)
{
  if (speed > 0)
  {
    rightF(speed);
  } else if (speed < 0)
  {
    rightB(speed * -1);
  } else
  {
    rightS();
  }
}

void setLeftMotor(int speed)
{
  if (speed > 0)
  {
    leftF(speed);
  } else if (speed < 0)
  {
    speed = speed * -1;
    leftB(speed);
  } else
  {
    leftS();
  }
}

void setBothMotor(int speed)
{
  setLeftMotor(speed);
  setRightMotor(speed);
}

void setMotors(int speed1, int speed2) 
{
  setLeftMotor(speed1);
  setRightMotor(speed2);
}

void stopAfterLeft(float rotations) 
{
  rotationsSr1 = 0; 
  while (rotationsSr1 < (rotations * 20)) {
    Serial.print("Left wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  leftS(); 
}

void stopAfterRight(float rotations) {
  rotationsSr2 = 0; 
  while (rotationsSr2 < (rotations * 20)) {
    Serial.print("Right wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  rightS(); 
}

void turnRight90() {
  setMotors(220, -220);
  stopAfterRight(0.65);
  allS();
}

void turnLeft90() {
  setMotors(-255, 255);
  stopAfterRight(0.25);
  allS();
}

#endif