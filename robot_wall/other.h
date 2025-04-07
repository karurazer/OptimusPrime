#ifndef OTHER_H
#define OTHER_H

#include "motor_control.h"

// check rotation sensor values using both digital and analog reads
void read_rotation_sensors() 
{
  if (millis() - sensorDataInterval >= timerSensor)
  {
    timerSensor = millis(); // reset timer
    int sr1_state = digitalRead(SR1); // digital read left wheel
    int sr2_state = digitalRead(SR2); // digital read right wheel
    int sr1_analog_state = analogRead(SR1); // analog read left wheel
    int sr2_analog_state = analogRead(SR2); // analog read right wheel

    Serial.print(" SR1 analog: ");
    Serial.print(sr1_analog_state);
    Serial.print(" ||Digital: ");
    Serial.print(sr1_state);

    Serial.print(" SR2 analog: ");
    Serial.print(sr2_analog_state);
    Serial.print(" ||Digital: ");
    Serial.println(sr2_state);
  }
}

// sets the black threshold for detecting the line
void callibrate_color()
{
  delay(2000); // wait before starting
  black = analogRead(A4) - 150; // calibrate value for black

  if (millis() - sensorDataInterval >= timerColor)
  {
    timerColor = millis(); // reset timer
    Serial.print(" Color value is: ");
    Serial.println(black); // print calibrated black value
  }
}

// read all sensor analog values into sensorValues[]
void read_color()
{
  for (int i = 0; i < NUM_SENSORS; i++) 
  {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

// check which sensors see black line based on threshold
void read_bool_color() 
{
  for (int i = 0; i < NUM_SENSORS; i++) 
  {
    whereIsLine[i] = analogRead(sensorPins[i]) > black;
  }
}

// return the distance measured by one ultrasonic sensor
int getDistanceFrom(int trigPin, int echoPin) 
{
  const int numSamples = 4;
  int readings[numSamples];

  for (int i = 0; i < numSamples; i++) 
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 6000);
    readings[i] = duration > 0 ? duration * 0.034 / 2 : 60;

    if (readings[i] > 50) 
    {
      readings[i] = 50; // cap distance to 50 cm
    }
    delay(2);
  }

  // sort readings to get middle value
  for (int i = 0; i < numSamples - 1; i++) 
  {
    for (int j = i + 1; j < numSamples; j++) 
    {
      if (readings[i] > readings[j]) 
      {
        int temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  return readings[numSamples / 2]; // return the median value
}

// returns true if object is farther than threshold
bool moreDistance(float threshold, int trig=TRIG, int echo=ECHO) 
{
  float distance = getDistanceFrom(trig, echo);
  return distance > 0 && distance >= threshold;
}

// print current distance to serial monitor
void show_distance(int trig=TRIG, int echo=ECHO) 
{
  float distance = getDistanceFrom(trig, echo);
  if (millis() - sensorDataInterval >= timerGoodDistance)
  {
    timerGoodDistance = millis(); // reset timer
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

// returns true if left sonar sees more than threshold
bool moreDistanceL(float threshold) 
{
  return moreDistance(threshold, TRIGL, ECHOL);
}

// returns true if right sonar sees more than threshold
bool moreDistanceR(float threshold) 
{
  return moreDistance(threshold, TRIGR, ECHOR);
}

// get front sonar reading
int getDistance() 
{
  return getDistanceFrom(TRIG, ECHO);
}

// get right sonar reading
int getDistanceR() 
{
  return getDistanceFrom(TRIGR, ECHOR);
}

// get left sonar reading
int getDistanceL() 
{
  return getDistanceFrom(TRIGL, ECHOL);
}

// move servo to a specific angle using pulse
void setServoAngle(int angle) 
{
  int pulseWidth = map(angle, 0, 180, 500, 2500); 

  digitalWrite(SERVO, HIGH);
  delayMicroseconds(pulseWidth); 
  digitalWrite(SERVO, LOW);
  delay(20);
}

// open the gripper
void open_servo() 
{
  setServoAngle(120);
  pulseWidth = 120;
}

// close the gripper
void close_servo() 
{
  setServoAngle(43);
  pulseWidth = 43;
}

// hold current servo position
void keep_servo() 
{
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interval) 
  {
    prevMillis = currentMillis;
    digitalWrite(SERVO, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(SERVO, LOW);
  }
}

// turning sequence based on distance from left and right
void turnAround() 
{ 
  left = getDistanceL();
  right = getDistanceR();

  int diff = map(abs(right - left), 0, 10, 10, 220); // adjust for angle
  setMotors(0, 0);
  delay(10);

  if (right > left) 
  {
    setMotors(0, -255); // spin in place to the left
    delay(900);
    setMotors(-255, -255); // reverse both motors
    delay(diff);
    setMotors(0, 0);
    delay(10);
    setMotors(255, 0); // move right motor forward to straighten
    delay(550);
  } 
  else 
  {
    setMotors(-255, 0); // spin in place to the right
    delay(900);
    setMotors(-255, -255); // reverse both
    delay(diff);
    setMotors(0, 0);
    delay(10);
    setMotors(0, 255); // move left motor forward to straighten
    delay(550);
  } 
  setMotors(255, 255); // move forward after turn
}

#endif
