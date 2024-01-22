#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SLAVE_ADDRESS_DHT 0x30

#define DHTPIN 2
#define DHTTYPE DHT11

float temperature = 0.0f;
DHT dht(DHTPIN, DHTTYPE);
#define YELLOW_PIN 4  // Broche LED jaune
#define GREEN_PIN 5   // Broche LED verte
#define RED_PIN 6     // Broche LED rouge

#define TEMP_MAX 28 // Seuil de température maximale
#define TEMP_MIN 11 // Seuil de température minimale

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adresse I2C de l'écran LCD

void requestEvent() {
  Wire.write((int)temperature);
  delay(1000);
  Serial.println(temperature);  // Imprimer la température sur le moniteur série
}

void readData() {
  float t = dht.readTemperature();
  if (!isnan(t)) {
    temperature = t;
  }
}

void computeData() {
  // Aucun calcul supplémentaire pour le capteur de température
}

void notifyData() {
  // Notification basée sur les seuils de température
  if (temperature > TEMP_MAX) {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
  } else if (temperature < TEMP_MIN) {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  } else {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
  }
}

void communicateData() {
  Wire.write((int)temperature);
  delay(100);
}

void displayTemperature() {
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print(" C");
  
  // Imprimer la température sur le moniteur série
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
}

void setup() {
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  

  Wire.begin(SLAVE_ADDRESS_DHT);
  Serial.begin(9600);
  dht.begin();
  Wire.onRequest(requestEvent);
}

void loop() {
  readData();
  computeData();
  notifyData();
  communicateData();
  displayTemperature();  // Ajout de l'affichage sur l'écran LCD et le moniteur série
  delay(1000);
}
