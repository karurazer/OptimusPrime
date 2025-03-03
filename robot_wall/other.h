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
void callibrate(){
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

// servo code
void open_servo() {
  myservo.write(180); 
}

void close_servo() {
  myservo.write(50);
}

void take_botle() {
  if (!withBotle) {
    setMotors(190, 190);
    if (!moreDistance(3)) {
      close_servo();
      withBotle = true;
      allS();
    }
  }
  return;
}

// end servo code

#endif