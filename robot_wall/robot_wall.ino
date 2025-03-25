#include "other.h"


int pos = 0;
void setup()
{
  Serial.begin(9600);
  myservo.attach(SERVO);

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
  allS(); // stop motors
  // callibrate(); // set black color
  // open_servo();
  close_servo();
  Serial.println(black);
  setMotors(255, 255);
  front = getDistance();
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

void rightTurn() {
  setMotors(255, 100);
  delay(500);
}

void leftTurn() {
  setMotors(0, 255);
  delay(700);
}

void leftTurnTest() {
  setMotors(100, 255);
  delay(500);
}


void rightTurnPro() {
  right = getDistanceR();
  int val = START_VALUE_RIGHT;
  int timerSensorTurn = 0;

  while (right > 7) { // Пока справа не будет препятствия
    if (millis() - RIGHT_PRO_TURN_TIME >= timerSensorTurn)
    {
      timerSensorTurn = millis();
      val += INCREASE_ON_RIGHT; 
    }
    setMotors(255, 255 - val); 
    right = getDistanceR();
    delay(10);
  }
}

void turn() {
  front = getDistance();
  right = getDistanceR();
  left = getDistanceL();

  if (FRONTDISTTURN >= front) {
    if (abs(right - left) < 5) {
      setMotors(0, -255 + (5 * 15));
      delay(500);
      setMotors(-255, -255);
      delay(500);
      setMotors(-255, 255);
    }
    else if (right > left) {
      setMotors(0, -255 + ((right - left) * 15));
      delay(500);
      setMotors(-255, -255);
      delay(500);
      setMotors(-255, 255);
    } else if (left > right) {
      setMotors(-255 + ((left - right) * 15), 0);
      delay(500);
      setMotors(-255, -255);
      delay(500);
      setMotors(255, -255);
    } 
    while (front < 20) {
        front = getDistance();
      }
  }
}

void loop()
{
  optimus_physical_walls();
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
  } else {
    setMotors(255, 255);
  }
  delay(10);
}

