#include "other.h"

int pos = 0; // used for servo position (not currently changing)

// calibrate servo and line sensor before start
void callibrate() 
{
  allS(); // stop all motors
  open_servo(); // open gripper
  delay(50);
  close_servo(); // close gripper
  delay(50);
  open_servo(); // open again for safety
  delay(50);

  callibrate_color(); // set black line threshold
}

void setup()
{
  // Serial.begin(9600); // for debugging if needed
  pinMode(SERVO, OUTPUT); // servo pin

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

  // attach interrupts to wheel encoder pins
  attachInterrupt(digitalPinToInterrupt(SR1), countRotationsSr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SR2), countRotationsSr2, FALLING);

  // run calibration routine
  callibrate();

  // start robot behavior
  start();

  // save initial front distance
  front = getDistance();
}

// turns right until the right sonar sees close enough wall
void rightTurn() 
{ 
  setMotors(255, 90); // turn slightly right
  while (getDistanceR() >= KEEPDISTANCE + 1) 
  {
    stop(); // check for black area
    delay(50);
  }
}

// hardcoded left turn
void leftTurn() 
{
  setMotors(0, 255);
  delay(700);
}

// test version of left turn
void leftTurnTest() 
{
  setMotors(100, 255);
  delay(500);
}

// test version of right turn
void rightTurnPro() 
{
  setMotors(255, 100);
}

// sequence for robot to start after object is placed
void start()
{
  allS(); // stop everything
  delay(200);
  front = getDistance();

  // wait for object to be placed close
  while (front > 20) 
  {
    delay(50);
    front = getDistance();
  }
  delay(200);

  // wait for object to be taken away
  while (front < 20) 
  {
    delay(50);
    front = getDistance();
  }

  delay(450); // short pause
  setMotors(255, 255); // move forward
  delay(950);
  close_servo(); // grab object
  leftTurn(); // turn into maze
  setMotors(255, 255); // move forward

  front = getDistance();
  right = getDistanceR();
  left = getDistanceL();

  // follow line while distances are safe
  while(right >= 15 && front >= 15 && left >= 15) 
  {
    optimus_followLine();
    front = getDistance();  
    right = getDistanceR();
    left = getDistanceL();
    delay(10);
  }
}

// check how many line sensors see black
int isBlack() 
{
  int numberBlack = 0;
  for (int i = 0; i < NUM_SENSORS; i++) 
  {
    if((analogRead(sensorPins[i]) > black)) 
    {
      numberBlack++;
    }
  }
  return numberBlack;
}

// function to check and react to black drop-off zone
void stop() 
{
  if (isBlack() >= 4) // black line detected by at least 4 sensors
  {
    if (!firstCheckDone) 
    {
      startBlack = millis(); // save time of first black detection
      firstCheckDone = true;
    }
  }

  if (firstCheckDone && millis() - startBlack >= waitBlack) 
  {
    if (isBlack() >= 4) 
    {
      setMotors(0, 0); // stop
      setMotors(-255, -255); // go backward a bit
      delay(200);
      open_servo(); // release object
      setMotors(-255, -255); // continue backward
      delay(1800);
      allS(); // stop completely

      while (true) 
      {
        delay(50); // wait forever
      }
    }
  }
}

// main loop
void loop()
{
  optimus_physical_walls(); // wall-following behavior
}

// wall-following navigation using distance sensors
void optimus_physical_walls() 
{ 
  if (millis() - previousMillis >= intervalMain) 
  {
    previousMillis = millis(); // reset timer
    
    front = getDistance();
    left = getDistanceL();
    right = getDistanceR();

    int change = map(abs(right - 10), 0, 10, 10, 200); // control sharpness of turn

    if (right > 22) 
    {
      rightTurn(); // wall gone right side
    } 
    else if (left > 22 && front <= 15) 
    { 
      leftTurn(); // open left and front blocked
    } 
    else if (front <= 13 && left <= 13 && right <= 13) 
    {
      turnAround(); // stuck, turn around
    } 
    else if (right >= KEEPDISTANCE - 1) 
    {
      setMotors(255, 255 - change); // adjust left
    } 
    else if (right <= KEEPDISTANCE + 1) 
    {
      setMotors(255 - change, 255); // adjust right
    } 
    else 
    {
      setMotors(255, 255); // go forward
    }

    stop(); // check for drop-off area
  }
}

// line-following logic using sensor values
void optimus_followLine() 
{
  read_bool_color(); // update line sensors

  if (whereIsLine[3] || whereIsLine[4]) 
  {
    setBothMotor(255); // go straight
  }
  else if (whereIsLine[5] || whereIsLine[6]) 
  {
    setMotors(190, 255); // slight right turn
  }
  else if(whereIsLine[7])
  {
    setMotors(170, 255); // sharper right turn
  }
  else if (whereIsLine[2] || whereIsLine[1]) 
  {
    setMotors(255, 190); // slight left turn
  }
  else if(whereIsLine[0]) 
  {
    setMotors(255, 170); // sharper left turn
  }
}
