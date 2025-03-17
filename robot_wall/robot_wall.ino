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
  // optimus_physical_walls();
  // setMotors(255, 255);
  test();
  // straightBack(5000);
  // show_distance(TRIGL, ECHOL);
  // turn();
  
}

void optimus_physical_walls() { // криво едет вперед если пусто с лево и кривые повороты
  front = getDistance();
  left = getDistanceL();
  right = getDistanceR();
  int diff = abs(left - right);
  
  if (right > 15) { // if space turn right
    rightTurn(); 
  }
  else if (front < 15 && left > 15) {
    leftTurn();
  }
  else if (front < 7 && left < 15 && right < 15) { // problem here
    if(right > left) {
      setMotors(-200, -255);
      delay(300);
      setMotors(255, -255);
    } else{ 
      setMotors(-255, -200);
      delay(300);
      setMotors(-255, 255);
    }
    while (front < 30) {
        front = getDistance();
      }
  }
  else if (500 < left || left < 3) { // if near left wall go right  problem
      setMotors(0, -255);
      delay(200);
      setMotors(255, 255);
      delay(200);
      setMotors(0, 255);
      delay(200);
  } 
  else if (500 < right || right < 3) { // if near right wall go left problem
      setMotors(-255, 0);
      delay(200);
      setMotors(255, 255);
      delay(200);
      setMotors(255, 0);
      delay(200);
  } 
  else if (left > 15) { // if space go forward check problem
    if (right < 10) {
      setMotors(200, 250);
    } else {
      setMotors(255, 200);
    }
  } 
  else if (left > right) { // forward problem mb
    if ((255 - (diff * 10)) < MINSPEED) {
      setMotors(MINSPEED, 255);
    } else {
      setMotors(255 - (diff * 10), 255);
    }
  } 
  else if (left < right) { // forward problem mb
     if ((255 - (diff * 10)) < MINSPEED) {
      setMotors(255, MINSPEED);
    } else {
      setMotors(255, 255 - (diff * 10));
    }
  } 
}

void straight(int time) {
  int now = millis(); 
  left = getDistanceL(); 
  
  while (millis() - now < time) { 
    if (left > 10) { 
      setMotors(220, 255); 
    } 
    else {
      setMotors(255, 220); 
    }
    
    delay(10); 
  }

  allS(); 
}
void straightBack(int time) {
  int now = millis();
  
  while (millis() - now < time) { 
    left = getDistanceL();  // Расстояние слева
    right = getDistanceR(); // Расстояние справа
    float diff = left - right;    // Разница между боковыми датчиками

    int baseSpeed = -255;  // Базовая скорость назад
    int correction = map(abs(diff), 0, 10, 0, 50); // Насколько корректировать
  
    if (left > right) { 
      setMotors(baseSpeed, baseSpeed - correction); // Подравниваем влево
    } 
    else if (right > left) {
      setMotors(baseSpeed- correction, baseSpeed); // Подравниваем вправо
    } 
    else {
      setMotors(baseSpeed, baseSpeed); // Двигаемся прямо
    }

    delay(10); 
  }

  allS(); // Останавливаемся по завершению
}

void turnAround() {
  front = getDistance();
  left = getDistanceL();
  right = getDistanceR();

  allS();
  delay(100);
  setMotors(-255, -255);
  delay(100);

  while (left < 25 || right < 25) {
    if (right < 10) { 
      setMotors(-255, -220);
    } 
    else {
      setMotors(-220, -255);
    }
  right = getDistanceR();
  left = getDistanceL(); 
  delay(10);
  }

  setMotors(-255, -255);
  delay(300);
  right = getDistanceR();
  if (right > 25) {
    leftTurn();
  }else {
    rightTurn();
  }
  straight(500);
}

void test() {
  front = getDistance();
  left = getDistanceL();
  right = getDistanceR();
  int diff = abs(left - right);
  
  if (right > 25) {
    rightTurn();
  }
  else if (left > 25 && front < 13) { 
    leftTurn();
  }
  else if (front < 13 && left < 13 && right < 13) {
    turnAround();
  }
  else  if (right > 10) { // чтобы вперед старый бог можно добавить более точное изменнение скорости с разницей
    setMotors(255, 220);
  } 
  else {
    setMotors(220, 255);
  }
}

