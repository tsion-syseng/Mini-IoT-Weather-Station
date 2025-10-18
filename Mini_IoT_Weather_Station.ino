#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Import the library for I2C LCD

// Sensor variables
float temp_vout, temp, voltage, rain, V_wind;
int gas_sensor_value, Windspeedint;
bool rainAlert = false, windAlert = false, tempAlert = false, gasAlert = false;

// Pin definitions
const int gas_sensor_port = A1;
const int triggerPin = 10;
const int echoPin = 9;
const int buzzerPin = 7;
const int temp_sensor_pin = A0;
const int wind_sensor_pin = A2;

// LCD initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(gas_sensor_port, INPUT);
  pinMode(temp_sensor_pin, INPUT);
  pinMode(wind_sensor_pin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  // Measure Rainfall
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  rain = 0.01723 * duration;
  rainAlert = (rain > 100);

  // Measure Wind Speed
  V_wind = analogRead(wind_sensor_pin) * (5.0 / 1023.0);
  Windspeedint = (V_wind - 0.4) * 20;
  windAlert = (Windspeedint > 10);

  // Measure Temperature
  temp_vout = analogRead(temp_sensor_pin);
  voltage = temp_vout * 0.0048828125;
  temp = (voltage - 0.5) * 100.0;
  tempAlert = (temp > 30);

  // Measure Gas Level
  gas_sensor_value = analogRead(gas_sensor_port);
  gasAlert = (gas_sensor_value > 200);

  // Prioritize and Display Alerts
  if (gasAlert) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas ALERT!");
    tone(buzzerPin, 1200, 2000);
    delay(2000);
  }
  if (tempAlert) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp ALERT!");
    tone(buzzerPin, 600, 2000);
    delay(2000);
  }
  if (rainAlert) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rain ALERT!");
    tone(buzzerPin, 1000, 2000);
    delay(2000);
  }
  if (windAlert) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wind ALERT!");
    tone(buzzerPin, 800, 2000);
    delay(2000);
  }

  // Display Normal Readings
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rain: "); lcd.print(rain); lcd.print("mm");
  lcd.setCursor(0, 1);
  lcd.print("Wind: "); lcd.print(Windspeedint); lcd.print("MPH");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: "); lcd.print(temp); lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Gas: "); lcd.print(gas_sensor_value);
  delay(2000);
}
