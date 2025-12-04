#include <LiquidCrystal.h>

// Define LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Define IR sensor and relay pins
const int irSensor1 = 2;  // First IR sensor (Entry)
const int irSensor2 = 3;  // Second IR sensor (Exit)
const int relay1 = 4;     // Relay for Fan
const int relay2 = 5;     // Relay for Light

int count = 0;
int lastDetected = 0;  // Stores the last sensor triggered

void setup() {
  lcd.begin(16, 2); // Initialize LCD
  lcd.print("System Ready"); // Initial message

  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, LOW); // Initially OFF
  digitalWrite(relay2, LOW); // Initially OFF

  delay(1000);
  lcd.clear();
}

void loop() {
  int sensor1State = !digitalRead(irSensor2);
  int sensor2State = !digitalRead(irSensor1);

  // Condition to turn ON both relays (Sensor 1 first, then Sensor 2)
  if (sensor2State == HIGH && lastDetected == 1) {
    count++;  // Increment count
    if (count == 1) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan and Light ON");
    lcd.setCursor(0, 1);
    lcd.print("Count: ");
    lcd.print(count);
    delay(500);
    lastDetected = 0;
    sensor2State=0;
  }

  // Condition to turn OFF both relays (Sensor 2 first, then Sensor 1)
  if (sensor1State == HIGH && lastDetected == 2) {
    if (count > 0) {
      count--;  // Decrement count
    }

    if (count == 0) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan and Light OFF");
    lcd.setCursor(0, 1);
    lcd.print("Count: ");
    lcd.print(count);
    delay(500);
    lastDetected = 0;
    sensor1State=0;
  }

  // Update last detected sensor
  if (sensor1State == HIGH) {
    lastDetected = 1;
  }

  if (sensor2State == HIGH) {
    lastDetected = 2;
  }
}
