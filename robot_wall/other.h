#ifndef OTHER_H
#define OTHER_H

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

// optimus wall code

// get current distance

//sonar code
int getDistanceFrom(int trigPin, int echoPin) {
  const int numSamples = 4;
  int readings[numSamples];

  for (int i = 0; i < numSamples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 6000);
    readings[i] = duration > 0 ? duration * 0.034 / 2 : 60;
    
    // Ограничение на 50 см
    if (readings[i] > 50) {
      readings[i] = 50;
    }
    delay(2);
  }

  
  for (int i = 0; i < numSamples - 1; i++) {
    for (int j = i + 1; j < numSamples; j++) {
      if (readings[i] > readings[j]) {
        int temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  return readings[numSamples / 2];
}

//  return true If distance more than threshold
bool moreDistance(float threshold, int trig=TRIG, int echo=ECHO) {
  float distance = getDistanceFrom(trig, echo);
  return distance > 0 && distance >= threshold;
}

void show_distance(int trig=TRIG, int echo=ECHO) {
  float distance = getDistanceFrom(trig, echo);
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

int getDistance() {
  return getDistanceFrom(TRIG, ECHO);
}

int getDistanceR() {
  return getDistanceFrom(TRIGR, ECHOR);
}

int getDistanceL() {
  return getDistanceFrom(TRIGL, ECHOL);
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
  pulseWidth = 120;
}

void close_servo() {
  setServoAngle(43);
  pulseWidth = 43;
}

void keep_servo() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;
    
    digitalWrite(SERVO, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(SERVO, LOW);
  }
}
  //end servo

void turnAround() { 
  left = getDistanceL();
  right = getDistanceR();
  int diff = map(abs(right - left), 0, 10, 10, 220);
  setMotors(0, 0);
  delay(10);

  if (right > left) {
    setMotors(0, -255);
    delay(900);
    setMotors(-255, -255);
    delay(diff);
    setMotors(0, 0);
    delay(10);
    setMotors(255, 0);
    delay(550);
  } else {
    setMotors(-255, 0);
    delay(900);
    setMotors(-255, -255);
    delay(diff);
    setMotors(0, 0);
    delay(10);
    setMotors(0, 255);
    delay(550);
  } 
  setMotors(255, 255);
}

#endif