#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
// PINS
#define MA1 10 // left 
#define MA2 11 // left

#define MB1 6 // right
#define MB2 5 // right

#define SR1 3 // left wheel
#define SR2 2 // right wheel

#define TRIG 9
#define ECHO 8
// motor values
const int pulses = 20;
int rotationsSr1 = 0;
int rotationsSr2 = 0;

// read sensor values
const int sensorDataInterval =  500;
int timerSensor = 0;

void countRotationsSr1() 
{
  rotationsSr1++; 
}
void countRotationsSr2() 
{
  rotationsSr2++; 
}
void printInfo(String info)
{
  if (millis() - sensorDataInterval >= timerSensor)
  {
    timerSensor = millis();
    Serial.println(info);
  }
}
void read_sensors() 
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
void right_s()
{
  digitalWrite(MB1, HIGH);
  digitalWrite(MB2, HIGH);
}
void left_s()
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, HIGH);
}
void all_s()
{
  left_s();
  right_s();
}
void left_f(int speed)
{
  analogWrite(MA2, speed);
    analogWrite(MA1, 0);
}
void right_f(int speed)
{
  analogWrite(MB1, speed);
    analogWrite(MB2, 0);
}
void right_b(int speed)
{
  analogWrite(MB2, speed);
    analogWrite(MB1, 0);
}
void left_b(int speed)
{
  analogWrite(MA1, speed);
    analogWrite(MA2, 0);
}
void forward(int speed)
{
  right_f(speed);
  left_f(speed);
}
void back(int speed)
{
  right_b(speed);
  left_b(speed);
} 
void setRightMotor(int speed)
{
  if (speed > 0)
  {
    right_f(speed);
  } else if (speed < 0)
  {
    right_b(speed * -1);
  } else
  {
    right_s();
  }
}
void setLeftMotor(int speed)
{
  if (speed > 0)
  {
    left_f(speed);
  } else if (speed < 0)
  {
    speed = speed * -1;
    left_b(speed);
  } else
  {
    left_s();
  }
}
void setBothMotor(int speed)
{
  setLeftMotor(speed);
  setRightMotor(speed);
}
void stopAfterLeft(float rotations) 
{
  rotationsSr1 = 0; 
  while (rotationsSr1 < (rotations * 20)) {
    Serial.print("Left wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  left_s(); 
}
void stopAfterRight(float rotations) {
  rotationsSr2 = 0; 
  while (rotationsSr2 < (rotations * 20)) {
    Serial.print("Right wheel rotations: ");
    Serial.println(rotationsSr2);
  }
  right_s(); 
}



float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2; 

  return distance;
}

bool stopDistance(int threshold) {
  float distance = getDistance();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= threshold) {
    Serial.println("Obstacle detected! Stopping.");
    return true;
  }
  return false;
}
bool moreDistance(int threshold) {
  float distance = getDistance();
  
  Serial.print("More than distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance >= threshold) {
    Serial.println("Distance is good.");
    return true;
  } 
  return false;
  
}
void turnOnDegrees(int degr){
  stopAfterRight(1);
}
#endif