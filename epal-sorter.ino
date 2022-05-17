#include <AccelStepper.h>

// Motor pin definitions:
#define motorPin1  8      // IN1 on the ULN2003 driver
#define motorPin2  9      // IN2 on the ULN2003 driver
#define motorPin3  10     // IN3 on the ULN2003 driver
#define motorPin4  11     // IN4 on the ULN2003 driver

// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

#define S0 3
#define S1 4
#define S2 6
#define S3 5
#define sensorOut 7

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() {
  Serial.begin(9600);
  
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  stepper.setMaxSpeed(1000);
}

void loop() {
  // Set the current position to 0:
  stepper.setCurrentPosition(0);

  // Run the motor forward at 500 steps/second until the motor reaches 4076 steps (1 revolution):
  while (stepper.currentPosition() != 512) { 
    stepper.setSpeed(500);
    stepper.runSpeed();
  }
  delay(2000);
  
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);  

  if ((redFrequency >= 50 && redFrequency <= 80) && greenFrequency >= 105) {
    Serial.println("Red");
  }

  if ((redFrequency >= 50 && redFrequency <= 80) && (greenFrequency >= 80 && greenFrequency <= 110)) {
    Serial.println("Orange");
  }

  if ((redFrequency >= 45 && redFrequency <= 65) && (greenFrequency <= 90 && blueFrequency <= 60)) {
    Serial.println("Yellow");
  }

  if ((redFrequency >= 105 && redFrequency <= 125) && (greenFrequency >= 95 && greenFrequency <= 135) && (blueFrequency >= 55 && blueFrequency <= 80)) {
    Serial.println("Brown");
  }

  if ((redFrequency >= 75 && redFrequency <= 95) && (greenFrequency >= 60 && greenFrequency <= 80) && (blueFrequency >= 45 && blueFrequency <= 60)) {
    Serial.println("Green");
  }

  if ((redFrequency >= 105 && redFrequency <= 125) && (greenFrequency >= 75 && greenFrequency <= 95) && (blueFrequency >= 35 && blueFrequency <= 55)) {
    Serial.println("Blue");
  }

  delay(1000);
}
