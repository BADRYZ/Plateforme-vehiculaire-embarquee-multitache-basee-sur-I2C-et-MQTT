#include <Wire.h>

#define SLAVE_ADDRESS 0x10 // Define slave address
const int BUTTON_RIGHT = 12, BUTTON_LEFT = 13;
char state;
const int LED[] = {3, 4, 5, 6};

void communicateData() {
  Wire.write(state); // Send the state of the buttons to the master
}

void setup() {
  for (int i = 3; i < 7; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(communicateData);
  Serial.begin(9600);
}

char readData() {
  char right_button_state = digitalRead(BUTTON_RIGHT);
  char left_button_state = digitalRead(BUTTON_LEFT);

  if (right_button_state == 1 && left_button_state == 1) {
    state = 'B';
  } else if (right_button_state == 1 && left_button_state == 0) {
    state = 'L';
  } else if (right_button_state == 0 && left_button_state == 1) {
    state = 'R';
  } else {
    state = '0';
  }

  return state;
}

void right_turnsignal() {
  for (int i = 3; i < 7; i++) {
    digitalWrite(i, HIGH);
    delay(70);
  }
  for (int i = 3; i < 7; i++) {
    digitalWrite(i, LOW);
  }
  delay(70);
}

void left_turnsignal() {
  for (int i = 7; i > 2; i--) {
    digitalWrite(i, HIGH);
    delay(70);
  }
  for (int i = 7; i > 2; i--) {
    digitalWrite(i, LOW);
  }
  delay(70);
}

void loop() {
  state = readData(); // Compute the state of the buttons
  Serial.println(state);
  
  if (state == 'L') {
    left_turnsignal();
  } else if (state == 'R') {
    right_turnsignal();
  }
}
