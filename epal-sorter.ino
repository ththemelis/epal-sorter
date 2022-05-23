#include <AccelStepper.h>   // Βιβλιοθήκη για τον χειρισμό του βηματικού κινητήρα
#include <Servo.h>    // Βιβλιοθήκη για τον χειρισμό του σερβοκινητήρα
#include <ArduinoQueue.h>

ArduinoQueue<int> queue(3);

// Ακροδέκτες σύνδεσης του κινητήρα
#define motorPin1  8      // IN1
#define motorPin2  9      // IN2
#define motorPin3  10     // IN3
#define motorPin4  11     // IN4

// Ο χειρισμός του κινητήρα θα γίνεται σε half step mode
#define MotorInterfaceType 8

AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);
Servo servoSorter;

// Ακροδέκτες σύνδεση του αισθητήρα χρώματος
#define S0 3
#define S1 4
#define S2 6
#define S3 5
#define sensorOut 7

// Αποθήκευση των τιμών για κάθε ένα βασικό χρώμα, από τον αισθητήρα
int red = 0;
int green  = 0;
int blue = 0;

int servoPositions[]= {50,80,110,140,165};
int servoPos;
int lastPos=0;
int lastValue;

void setup() {
  Serial.begin(9600);
  
  // Ορισμός των ακροδεκτών του αισθητήρα χρώματος ως έξόδους
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Ορισμός του ακροδέκτη του αισθητήρα χρώματος ως είσοδο
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  stepper.setMaxSpeed(1000);
  servoSorter.attach(2);
  servoSorter.write(90);
}

void loop() {
  stepper.setCurrentPosition(0);

  while (stepper.currentPosition() != 512) { 
    stepper.setSpeed(500);
    stepper.runSpeed();
  }
  delay(200);
  
  // Προετοιμασία για ανάγνωση κόκκινου χρώματος
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  red = pulseIn(sensorOut, LOW);  // Ανάγνωση κόκκινου χρώματος
  delay(100);
    
  // Προετοιμασία για ανάγνωση πράσινου χρώματος
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  green  = pulseIn(sensorOut, LOW); // Ανάγνωση πράσινου χρώματος
  delay(100);
  
  // Προετοιμασία για ανάγνωση μπλε χρώματος
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  blue = pulseIn(sensorOut, LOW); // Ανάγνωση μπλε χρώματος
  delay(100);
    
   // Εκτύπωση των τιμών των χρωμάτων που διάβασε ο αισθητήρας
  Serial.print("R= ");
  Serial.print(red);
  Serial.print(" G= ");
  Serial.print(green );
  Serial.print(" B= ");
  Serial.print(blue);
  Serial.print("  ");

  if ((red >= 25 && red <= 50) && (green  >= 75 && green  <= 95) && (blue >= 40 && blue <= 60)) {
    Serial.print("Red");
    servoPos = servoPositions[0];
  }
  else if ((red >= 25 && red <= 50) && green  <= 45) {
    Serial.print("Yellow");
    servoPos = servoPositions[0];
  }
  else if ((red >= 25 && red <= 50) && green  <= 70) {
    Serial.print("Orange");
    servoPos = servoPositions[0];
  }
  else if ((red >= 70 && red <= 95)  && green  >= 75 && blue >= 40 ) {
    Serial.print("Brown");
    servoPos = servoPositions[2];
  }
  else if ((red >= 55 && red <= 70) && (green  >= 45 && green  <= 60)) {
    Serial.print("Green");
    servoPos = servoPositions[3];
  }
  else if ((red >= 70 && red <= 95) && (green  >= 50 && green  <= 65) && blue <= 35) {
    Serial.print("Blue");
    servoPos = servoPositions[4];
  }
  Serial.println("");
  delay(1000);

  queue.enqueue(servoPos);
  if (queue.isFull()) {
    lastValue = queue.dequeue();     
    if (lastPos < lastValue) {
      for (int i=lastPos; i<=lastValue; i++) {
        servoSorter.write(i);
        delay(15);
      }
    }
  else {
    if (lastValue==0) {
      return;
    }
    else {
    for (int i=lastPos; i>=lastValue; i--) {
        servoSorter.write(i);
        delay(15);
    }
   }
  }
  }
  lastPos = lastValue;
}
