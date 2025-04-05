#include "other.h"


int pos = 0; // add servo keep

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
  setMotors(255, 90);
  while (getDistanceR() >= KEEPDISTANCE + 1) {
    stop();
    delay(50);
  }
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
  setMotors(255, 100);
}

void start(){
  allS();
  delay(200);
  front = getDistance();

  while (front > 20) {
    delay(50);
    front = getDistance();
  }
  delay(200);

  while (front < 20) {
    delay(50);
    front = getDistance();
  }
  delay(450);
  setMotors(255, 255);
  delay(950);
  close_servo();
  leftTurn();
  setMotors(255, 255);
  front = getDistance();
  right = getDistanceR();
  left = getDistanceL();

  while(right >= 15 && front >= 15 && left >= 15) {
    optimus_followLine();
    front = getDistance();  
    right = getDistanceR();
    left = getDistanceL();
    delay(10);
  }
}

int isBlack() {
  int numberBlack = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
      if((analogRead(sensorPins[i]) > black)) {
        numberBlack++;
      }
  }

  return numberBlack;
}

void stop() {
  // Проверяем, если isBlack() возвращает true (черная линия)
  if (isBlack() >= 4) {
    // Если первая проверка не была сделана
    if (!firstCheckDone) {
      startBlack = millis(); // Запоминаем время первого обнаружения черной линии      
      firstCheckDone = true; // Устанавливаем флаг, что первая проверка выполнена
  }
}

  // Если прошло достаточно времени для повторной проверки
  if (firstCheckDone && millis() - startBlack >= waitBlack) {
    // Если черная линия снова обнаружена
    if (isBlack() >= 4) {
      setMotors(0, 0);
      setMotors(-255, -255);
      delay(200);
      open_servo();
      setMotors(-255, -255);
      delay(1800);
      allS();
      while (true) {
        delay(50);
      }
    }
  }
}

void loop()
{
  optimus_physical_walls();
}

void optimus_physical_walls() { 
  if (millis() - previousMillis >= intervalMain) {
    previousMillis = millis();
    
    front = getDistance();
    left = getDistanceL();
    right = getDistanceR();

    int change = map(abs(right - 10), 0, 10, 10, 200); // 10 mid pos

    if (right > 22) {
      rightTurn();
    } 
    else if (left > 22 && front <= 15) { 
      leftTurn();
    } 
    else if (front <= 13 && left <= 13 && right <= 13) {
      turnAround();
    } 
    else if (right >= KEEPDISTANCE - 1) { // как сделать движение вперед стабильнее  
      setMotors(255, 255 - change);
    } 
    else if (right <= KEEPDISTANCE + 1) {
      setMotors(255 - change, 255);
    } 
    else {
      setMotors(255, 255);
    }

    stop();
    }
  }

  void optimus_followLine() { // try change values in order to increase speed
    read_bool_color();
    if (whereIsLine[3] || whereIsLine[4]) {
      setBothMotor(255);
    }
    else if (whereIsLine[5] || whereIsLine[6]) {
      setMotors(190, 255);
    }
    else if(whereIsLine[7]){
      setMotors(170, 255);
    }
    else if (whereIsLine[2] || whereIsLine[1]) {
      setMotors(255, 190);
    }
    else if(whereIsLine[0]) {
      setMotors(255, 170);
  }
}