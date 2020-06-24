/*
Cassandra Chow - Embedded Sentry Project

Started May 11, 2020
Finished May 19, 2020

Average of yGyro readings must be negative to start recording lock
MPU recognizes directions through changes in acceleration/assign each direction a number
Get MPU to print out directions with the axis readings (optional for final product) and assign number of index in direct array
Make sure the numbers are performed in that exact sequence by checking the direct array
 */

#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, t;
const int moves = 5;                //restricts number of moves user is allowed. can change
int movesRecorded = 0;
const int average = 5;              //takes the average of 5 readings
const float threshold = 3;

//Original average of acceleration
float xAvg;
float yAvg;
float zAvg;

//Used to find the average of acceleration and gyro readings
float xAcc[average];
float yAcc[average];
float zAcc[average];
float yGyro[average];

int direct[moves];                  //int array of directions recorded for lock

void reading() {
  mpu.getEvent(&a, &g, &t);

  for (int i = 0; i < average; i++) {
    xAcc[i] = a.acceleration.x;
    yAcc[i] = a.acceleration.y;
    zAcc[i] = a.acceleration.z;
    yGyro[i] = g.gyro.y;
  }
}

//For test purposes
void printArray(float* arrPtr) {
  Serial.println();
  Serial.print("Array: ");
  for (int i = 0; i < average; i++) {
    Serial.print(arrPtr[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void printArrayInt(int* arrPtr) {
  Serial.println();
  Serial.print("Array (int): ");
  for (int i = 0; i < average; i++) {
    Serial.print(arrPtr[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

float findaverage(float* arrPtr) {
  reading();
  float sum = 0;
  for (int i = 0; i < average; i++) { sum = sum + arrPtr[i]; }
  return sum/average;
}

//Sets global variables: acceleration
void newReadings() {
  Serial.println(" ");
  Serial.println("Please wait. Calibrating MPU. Do not touch.");
  reading();
  xAvg = findaverage(xAcc);
  yAvg = findaverage(yAcc);
  zAvg = findaverage(zAcc);
  Serial.println("Done.");
  Serial.println(" ");
}

void record() {
  mpu.getEvent(&a, &g, &t);
  float currX = a.acceleration.x;
  float currY = a.acceleration.y;
  float currZ = a.acceleration.z;
  int moveDir;
  
  //Find MPU direction using acceleration
  if ((currZ - zAvg) > threshold) {
    Serial.println("Up");
    moveDir = 4;
  }
  else if ((zAvg - currZ) > threshold) {
    Serial.println("Down");
    moveDir = 5;    
  }
  else if ((currY - yAvg) > threshold) {
    Serial.println("Forward"); //need to check
    moveDir = 2;
  }
  else if ((yAvg - currY) > threshold) {
    Serial.println("Backward");
    moveDir = 3;
  }
  else if ((currX - xAvg) > threshold) {
    Serial.println("Right");
    moveDir = 0;
  }
  else if ((xAvg - currX) > threshold) {
    Serial.println("Left");
    moveDir = 1;
  }
  else {
    return; //Don't store empty moves 
  }
  direct[movesRecorded] = moveDir;
  movesRecorded = movesRecorded + 1;
  delay(1000);
}

bool attempt() {
  mpu.getEvent(&a, &g, &t);
  float currX = a.acceleration.x;
  float currY = a.acceleration.y;
  float currZ = a.acceleration.z;
  int moveDir;
  int i = 0;
  int j = 0;
  int directAttempt[movesRecorded];

  while (i < movesRecorded) {
    mpu.getEvent(&a, &g, &t);
    currX = a.acceleration.x;
    currY = a.acceleration.y;
    currZ = a.acceleration.z;

    if ((currZ - zAvg) > threshold) {
      Serial.println("Up");
      moveDir = 4;
    }
    else if ((zAvg - currZ) > threshold) {
      Serial.println("Down");
      moveDir = 5;
    }
    else if ((currY - yAvg) > threshold) {
      Serial.println("Forward"); //need to check
      moveDir = 2;
    }
    else if ((yAvg - currY) > threshold) {
      Serial.println("Backward");
      moveDir = 3;
    }
    else if ((currX - xAvg) > threshold) {
      Serial.println("Right");
      moveDir = 0;
    }
    else if ((xAvg - currX) > threshold) {
      Serial.println("Left");
      moveDir = 1;
    }
    else {
      //Serial.println("Else");
      continue; //Don't store empty moves 
    }
    directAttempt[i] = moveDir;
    i = i + 1;
    delay(1000);
  }
  
  while (directAttempt[j] == direct[j]) {
    j = j + 1;
    if (j == 5) { return 1; }
  }
  return 0;
}

void setup(void) {
  //Serial and MPU setup
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  while (!Serial) { delay(10); } // will pause Zero, Leonardo, etc until serial console opens
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("Welcome to the Embedded Sentry!");

  Serial.println(" ");
  Serial.print("You have: ");
  Serial.print(moves);
  Serial.println(" moves.");

  Serial.println(" ");
  Serial.println("When inputting directions, make movement clear, stop for one second, and record another direction.");
  
  newReadings(); //Reset xAvg, yAvg, zAvg
  delay(100);

  Serial.println("Please enter your locking sequence:");
  Serial.println(" ");
  
  //While right side up: record sequence
  while ((movesRecorded < moves)) { //(findaverage(yGyro) >= -0.30) && 
    record();
  }
  Serial.println(" ");
  Serial.println("You have locked the Embedded Sentry.");
  Serial.println(" ");
  Serial.println("Please enter the locking sequence: ");
  Serial.println(" ");
}

void loop() {
  while (!attempt()) { //While user was not able to unlock sequence
    Serial.println(" ");
    Serial.println("Please enter the correct sequence.");
    Serial.println(" ");
  }

  //User was able to unlock sequence
  Serial.println(" ");
  Serial.println("You have unlocked the Embedded Sentry.");
  Serial.println(" ");
  Serial.println("To try again, please press the button the Arduino Uno.");
  delay(100);
  while(1);
}
