#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
// PINS
#define MA1 10 // left 
#define MA2 11 // left
#define ENA 00

#define MB1 6 // right
#define MB2 5 // right
#define ENB 00

#define SR1 3 // left wheel
#define SR2 2 // right wheel

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
  digitalWrite(MB1, LOW);
  digitalWrite(MB2, LOW);
}
void left_s()
{
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, LOW);
}
void all_s()
{
  left_s();
  right_s();
}
void left_f()
{
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, HIGH);
}
void right_f()
{
  digitalWrite(MB1, HIGH);
  digitalWrite(MB2, LOW);
}
void right_b()
{
  digitalWrite(MB1, LOW);
  digitalWrite(MB2, HIGH);
}
void left_b()
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, LOW);
}
void forward()
{
  right_f();
  left_f();
}
void back()
{
  right_b();
  left_b();
} 
void setRightMotor(int speed)
{
  if (speed > 0)
  {
    right_f();
  } else if (speed < 0)
  {
    right_b();
  } else
  {
    right_s();
  }
  analogWrite(ENB, speed);
}
void setLeftMotor(int speed)
{
  if (speed > 0)
  {
    left_f();
  } else if (speed < 0)
  {
    left_b();
  } else
  {
    left_s();
  }
  analogWrite(ENA, speed);
}
void setBothMotor(int speed)
{
  setLeftMotor(speed);
  setRightMotor(speed);
}
// void stop_after(int rotations) 
// {
//   int count = 0;

//   while (count < rotations) 
//   { 
//     if (digitalRead(SR1) == LOW || digitalRead(SR2) == LOW) 
//     {
//       count++;
//       Serial.print("Rotation count: ");
//       Serial.println(count);
//       delay(50);  
//     }
//   }
//   all_s();
// }

#endif