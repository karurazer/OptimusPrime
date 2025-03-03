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
  callibrate(); // set black color
  open_servo();
  Serial.println(black);
}

void optimus_avoiding_object(){
  if (!moreDistance(20)){
    allS();
    delay(30);
    leftF(250);
    stopAfterLeft(0.75);
    forward(250);
    delay(500);
    allS();
    rightF(250);
    stopAfterRight(0.75);
    forward(250);
    delay(1000);
    allS();
    rightF(250);
    stopAfterRight(0.75);
    forward(250);
    delay(500);
    allS();
    leftF(250);
    stopAfterLeft(0.75);

  } else if(moreDistance(40)){
    setBothMotor(250);
  }
}

void otimus_basic_move(){
  forward(250);
  stopAfterRight(5);
  allS();
  back(250);
  stopAfterRight(5);
  allS();
  delay(1000);
  rightF(250);
  stopAfterRight(0.75);
  delay(500);
  leftF(250);
  stopAfterLeft(0.75);
  delay(10000);
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

void optimus_wall() {
  if (!moreDistance(15)) {
    
  } 
  else if(moreDistance(20)){
    setBothMotor(250);
  }
}

void loop()
{
  show_distance(TRIGR, ECHOR);
}



