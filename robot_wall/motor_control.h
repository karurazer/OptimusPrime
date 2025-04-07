#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// PINS
#define SERVO 4 // Servo motor pin for the gripper

#define NUM_SENSORS 8  
int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7}; // IR line sensors connected to analog pins
int sensorValues[NUM_SENSORS]; // store analog values from line sensors
bool whereIsLine[NUM_SENSORS];  // array to store detected line position
bool prev[NUM_SENSORS]; // stores previous sensor states
int black = 0; // used for black line detection

// Motor pins
#define MA1 10 // left motor control pin 1
#define MA2 11 // left motor control pin 2

#define MB1 6 // right motor control pin 1
#define MB2 5 // right motor control pin 2

// Encoder pins (for counting wheel rotations)
#define SR1 0 // left wheel encoder - currently not connected
#define SR2 3 // right wheel encoder

// Sonar pins for distance measurement
#define TRIG 9
#define ECHO 8

#define TRIGL 13
#define ECHOL 12

#define TRIGR 7
#define ECHOR 2

// Constants for turning control
#define RIGHT_PRO_TURN_TIME 50
#define INCREASE_ON_RIGHT 20
#define START_VALUE_RIGHT 10;

#define LEFT_PRO_TURN_TIME 100
#define INCREASE_ON_LEFT 60
#define START_VALUE_LEFT 40

// Distance keeping threshold
#define KEEPDISTANCE 9

// Minimum motor speed to move the robot
#define MINSPEED 170

// motor values
const int pulses = 20; // encoder pulses per rotation
int rotationsSr1, rotationsSr2 = 0; // count wheel rotations
int diameter = 6.5; // wheel diameter in cm

// distance readings
float front;
float left;
float right;

// sensor timers
const int sensorDataInterval = 500; // interval to read sensor data
int timerSensor, timerMoreDistance, timerGoodDistance, timerColor = 0;

// Servo settings
int pulseWidth = 0; 
unsigned long prevMillis = 0;
const int interval = 20; // time interval for servo update

// main loop timer
unsigned long previousMillis = 0;
unsigned long intervalMain = 50;

// black object detection
int waitBlack = 200;
unsigned long startBlack = 0;
bool firstCheckDone = false; 

// function to count left wheel encoder pulses
void countRotationsSr1() 
{
  rotationsSr1++; 
}

// function to count right wheel encoder pulses
void countRotationsSr2() 
{
  rotationsSr2++; 
}

// stop right motor
void rightS()
{
  analogWrite(MB1, 0);
  analogWrite(MB2, 0);
}

// stop left motor
void leftS()
{
  analogWrite(MA1, 0);
  analogWrite(MA2, 0);
}

// stop both motors
void allS()
{
  leftS();
  rightS();
}

// move left motor forward
void leftF(int speed)
{
  analogWrite(MA2, speed);
  analogWrite(MA1, 0);
}

// move right motor forward
void rightF(int speed)
{
  analogWrite(MB1, speed);
  analogWrite(MB2, 0);
}

// move right motor backward
void rightB(int speed)
{
  analogWrite(MB2, speed);
  analogWrite(MB1, 0);
}

// move left motor backward
void leftB(int speed)
{
  analogWrite(MA1, speed);
  analogWrite(MA2, 0);
}

// move both motors forward
void forward(int speed)
{
  rightF(speed);
  leftF(speed);
}

// move both motors backward
void back(int speed)
{
  rightB(speed);
  leftB(speed);
} 

// set speed for right motor (positive = forward, negative = backward)
void setRightMotor(int speed)
{
  if (speed > 0)
  {
    rightF(speed);
  } 
  else if (speed < 0)
  {
    rightB(speed * -1); // make speed positive
  } 
  else
  {
    rightS(); // stop if speed = 0
  }
}

// set speed for left motor (positive = forward, negative = backward)
void setLeftMotor(int speed)
{
  if (speed > 0)
  {
    leftF(speed);
  } 
  else if (speed < 0)
  {
    speed = speed * -1; // make speed positive
    leftB(speed);
  } 
  else
  {
    leftS(); // stop if speed = 0
  }
}

// set same speed for both motors
void setBothMotor(int speed)
{
  setLeftMotor(speed);
  setRightMotor(speed);
}

// set custom speeds for left and right motor
void setMotors(int speed1, int speed2) 
{
  setLeftMotor(speed1);
  setRightMotor(speed2);
}

// stop left motor after specific number of rotations
void stopAfterLeft(float rotations) 
{
  rotationsSr1 = 0; 
  while (rotationsSr1 < (rotations * 20)) // convert rotations to pulses
  {
    Serial.print("Left wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  leftS(); 
}

// stop right motor after specific number of rotations
void stopAfterRight(float rotations) 
{
  rotationsSr2 = 0; 
  while (rotationsSr2 < (rotations * 20)) 
  {
    Serial.print(" Right wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  rightS(); 
}

// turn right 90 degrees using motor control and encoder feedback
void turnRight90() 
{
  setMotors(220, -220); // left forward, right backward = right turn
  stopAfterRight(0.65); // stop after 0.65 rotations (can be adjusted)
  allS();
}

// turn left 90 degrees using motor control and encoder feedback
void turnLeft90() 
{
  setMotors(-255, 255); // left backward, right forward = left turn
  stopAfterRight(0.25); // stop after 0.25 rotations (can be adjusted)
  allS();
}

#endif
