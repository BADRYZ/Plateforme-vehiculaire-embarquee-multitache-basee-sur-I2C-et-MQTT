#include <Wire.h>
#define SLAVE_ADDRESS_ULTRASONIC 0x20
int DELAY = 500;
const int RPIN = 6, GPIN = 5, BPIN = 4;
const int TRIGGER_PIN = 11, ECHO_PIN = 12;
const int Dmin = 7, Dmax = 20;
const float sound_speed = 0.0343;
float distance , calculatedSpeed;
char Ultrasonic_values[14];

String COLOR;

//--------------------------------------------------------------------
void setup() {
  Wire.begin(SLAVE_ADDRESS_ULTRASONIC);
  Wire.onRequest(communicateDataTask);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

//--------------------------------------------------------------------
void loopApp() {
  float distance = collectDataTask();
  String COLOR = computeDataTask(distance);
  notifyDataTask(COLOR);
  communicateDataTask();
  timeDelayTask(DELAY);
}

// TASKS --------------------------------------------------------------------
float collectDataTask() {
  Serial.print("\n\nCOLLECT DATA TASK: ");
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration / 29.1 /2 ;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  return distance;
}

String computeDataTask(float distance) {
  Serial.print("\nCOMPUTE DATA TASK: ");
  if (distance < Dmin) {
    COLOR = "RED";
    calculatedSpeed = 0;
  } else if (distance > Dmax) {
    COLOR = "GREEN";
    calculatedSpeed = ((distance - Dmin) * 60 / (Dmax - Dmin));
  } else if ( distance <= Dmax && distance >= Dmin) {
    COLOR = "BLUE";
    calculatedSpeed = 60;
  }
  Serial.print("DISTANCE OF COLOR: ");
  Serial.print(COLOR);
  return COLOR;
}

void notifyDataTask(String COLOR) {
  Serial.print("\nNOTIFY DATA TASK: ");
  if (COLOR == "BLUE"){
    analogWrite(RPIN, 0);
    analogWrite(GPIN, 0);
    analogWrite(BPIN, 255);  
  }
  else if (COLOR == "RED"){
    analogWrite(RPIN, 255);
    analogWrite(GPIN, 0);
    analogWrite(BPIN, 0);
  }
  else{
    analogWrite(RPIN, 0);
    analogWrite(GPIN, 255);
    analogWrite(BPIN, 0);
  }
  Serial.print("LIGHTENING ");
  Serial.print(COLOR);
  Serial.print(" LED");
}

void communicateDataTask() {
  Serial.print("\nCOMMUNICATE DATA TASK: ");
  String speedStr = String(calculatedSpeed, 2);
  String distanceStr = String(distance,2);
  String DataTotal = (speedStr+";"+distanceStr+";");
  DataTotal.toCharArray(Ultrasonic_values, 14);
  Wire.write(Ultrasonic_values);
  delay(100);
}

void timeDelayTask(int DELAY) {
  Serial.print("\nTIME DELAY TASK: ......");
  delay(DELAY);
}

void loop() {
  loopApp();
}