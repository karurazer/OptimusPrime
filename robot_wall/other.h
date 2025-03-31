#ifndef other_h
#define other_h

#include "motor_control.h"

// rotation sensor
void read_rotation_sensors() 
{
  if (millis() - sensorDataInterval >= timerSensor)
  {
    timerSensor = millis();
    int sr1_state = digitalRead(SR1);
    int sr2_state = digitalRead(SR2);
    int sr1_analog_state = analogRead(SR1);
    int sr2_analog_state = analogRead(SR2);
    Serial.print("SR1 analog: ");
    Serial.print(sr1_analog_state);
    Serial.print("||Digital: ");
    Serial.print(sr1_state);

    Serial.print("   SR2 analog: ");
    Serial.print(sr2_analog_state);
    Serial.print("||Digital: ");
    Serial.println(sr2_state);
  }
}

// follow line code
void callibrate_color(){
  delay(2000);
  black = analogRead(A4) - 150;

  if (millis() - sensorDataInterval >= timerColor)
  {
    timerColor = millis();
    Serial.print("Color value is: ");
    Serial.println(black);
  }
}

void read_color(){
    for (int i = 0; i < NUM_SENSORS; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void read_bool_color() {
  for (int i = 0; i < NUM_SENSORS; i++) {
      whereIsLine[i] = analogRead(sensorPins[i]) > black;
  }
}

// end follow line code

// get current distance
float getDistance(int trig=TRIG, int echo=ECHO) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration = pulseIn(echo, HIGH);
  float distance = duration * 0.034 / 2; 

  return distance;
}
  //sonar code
//  return true If distance more than threshold
bool moreDistance(float threshold, int trig=TRIG, int echo=ECHO) {
  float distance = getDistance(trig, echo);
  return distance > 0 && distance >= threshold;
}

void show_distance(int trig=TRIG, int echo=ECHO) {
  float distance = getDistance(trig, echo);
  if (millis() - sensorDataInterval >= timerGoodDistance)
        {
          timerGoodDistance = millis();
          Serial.print("Distance: ");
          Serial.print(distance);
          Serial.println(" cm");
        }
}

bool moreDistanceL(float threshold) {
  return moreDistance(threshold, TRIGL, ECHOL);
}

bool moreDistanceR(float threshold) {
  return moreDistance(threshold, TRIGR, ECHOR);
}

float getDistanceL() {
  return getDistance(TRIGL, ECHOL);
}

float getDistanceR() {
  return getDistance(TRIGR, ECHOR);
}

  //end sonar

  // servo code
void setServoAngle(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2500); 
  
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(pulseWidth); 
  digitalWrite(SERVO, LOW);
  delay(20);
}

void open_servo() {
  setServoAngle(120);
}

void close_servo() {
  setServoAngle(43);
}
  //end servo

void turnAround() { 
  left = getDistanceL();
  right = getDistanceR();
  int diff = map(abs(right - left), 0, 16, 0, 270);
  delay(10);

  if (right > left) {
    setMotors(0, -255);
    delay(900);
    setMotors(-255, -255);
    delay(diff);
    setMotors(255, 0);
    delay(700);
  } else {
    setMotors(-255, 0);
    delay(900);
    setMotors(-255, -255);
    delay(diff);
    setMotors(0, 255);
    delay(700);
  } 
  setMotors(255, 255);
  delay(10);
}





#endif