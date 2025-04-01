#include "other.h"


int pos = 0;

void callibrate() {
  allS();
  open_servo();
  delay(50);
  close_servo();
  delay(50);
  open_servo();
  delay(50);

  callibrate_color();
}

void setup()
{
  // Serial.begin(9600);
  pinMode(SERVO, OUTPUT);

  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);

  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);

  pinMode(SR1, INPUT);
  pinMode(SR2, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(TRIGR, OUTPUT);
  pinMode(ECHOR, INPUT);

  pinMode(TRIGL, OUTPUT);
  pinMode(ECHOL, INPUT);

  attachInterrupt(digitalPinToInterrupt(SR1), countRotationsSr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SR2), countRotationsSr2, FALLING);

  // callibrating
  callibrate(); // calibrate servo and motors
  start();
  front = getDistance();
}

void rightTurn() { 
  setMotors(255, 100);
  delay(500);
}

void leftTurn() {
  setMotors(0, 255);
  delay(700);
}

void leftTurnTest() { // test
  setMotors(100, 255);
  delay(500);
}

void rightTurnPro() { // test
  right = getDistanceR();
  int val = START_VALUE_RIGHT;
  int timerSensorTurn = 0;

  while (right > 7) { // Пока справа не будет препятствия
    if (millis() - RIGHT_PRO_TURN_TIME >= timerSensorTurn)
    {
      timerSensorTurn = millis();
      val += INCREASE_ON_RIGHT; 
    }
    setMotors(255, 200 - val); 
    right = getDistanceR();
    delay(10);
  }
}

void start(){
  allS();
  delay(200);
  front = getDistance();

  while (front > 10) {
    delay(10);
    front = getDistance();
  }
  delay(200);

  while (front < 10) {
    delay(10);
    front = getDistance();
  }

  setMotors(255, 255);
  delay(1000);
  close_servo();
  leftTurn();
  setMotors(255, 255);
  front = getDistance();
  right = getDistanceR();
  left = getDistanceL();

  while(right >= 15 && front >= 15 && left >= 15) {
    front = getDistance();
    right = getDistanceR();
    left = getDistanceL();
    delay(10);
  }
}

void stop() {
  read_bool_color();
  for (bool i : whereIsLine) {
    if (!i) {
      return;
    }
  }

  open_servo();
  setMotors(-255, -255);
  delay(2000);
  while (true) {
    allS();
    delay(50);
  }
}
void loop()
{
  optimus_physical_walls();
  // Serial.println(getDistanceL());
}



void optimus_physical_walls() { 
  front = getDistance();
  left = getDistanceL();
  right = getDistanceR();
  int change = map(abs(right - 10), 0, 10, 10, 200);
  
  if (right > 25) {
    rightTurn();
  }
  else if (left > 25 && front <= 14) { 
    leftTurn();
  }
  else if (front <= 13 && left <= 13 && right <= 13) {
    turnAround();
  }
  else  if (right > 11) { 
    setMotors(255, 255 - change);
  }
  else if (right < 9) {
    setMotors(255 - change, 255);
  } 
  else {
    setMotors(255, 255);
  }
  delay(10);
  stop();
}

void optimus_followLine_final(){ // try change values in order to increase speed
  read_bool_color();
  if (whereIsLine[3] || whereIsLine[4]) {
    setBothMotor(255);
  }
  else if (whereIsLine[5] || whereIsLine[6]) {
    setMotors(170, 255);
  }
  else if(whereIsLine[7]){
    setMotors(0, 255);
  }
  else if (whereIsLine[2] || whereIsLine[1]) {
    setMotors(255, 170);
  }
  else if(whereIsLine[0]) {
    setMotors(255, 0);
  }
}