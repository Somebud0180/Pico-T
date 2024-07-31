// Now turn this trash into treasure!

#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 27
#define DHTTYPE DHT22
#define ENCODER_CLK 13
#define ENCODER_DT  14
#define ENCODER_BTN 15

const int pinR = 20;
const int pinG = 19;
const int pinB = 18;
const int buttonPin = 28;
int oldButton = LOW;
int lastClk = HIGH;
float hotTemp = 32;
float warmTemp = hotTemp - 2;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial1.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, FALLING);
  
  lcd.display();
  lcd.begin(16, 2);
  lcd.print("Press the button");

  lcd.setCursor(0, 1);
  lcd.print("to check temps");

  delay(4000);
  lcd.clear();
  lcd.noDisplay();
  dht.begin();
}

void readEncoder() {
  int dtValue = digitalRead(ENCODER_DT);
  if (dtValue == HIGH) {
    hotTemp++;
    Serial1.println(hotTemp);
  }
  if (dtValue == LOW) {
    hotTemp--;
    Serial1.println(hotTemp);
  }
  warmTemp = hotTemp - 2;
}

void loop() {
  //Humidity
  float humid = dht.readHumidity();
  //Temperature (Celcius)
  float temp = dht.readTemperature();
  // Compute heat index (Celcius)
  float feelsLike = dht.computeHeatIndex(temp, humid, false);

  // Check if sensor failed and try again.
  if (isnan(humid) || isnan(temp)) {
    lcd.display();
    lcd.print(F("Failed to read"));
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor!");
    delay(2000);
    lcd.clear();
    lcd.noDisplay();
    return;
  }

  if (digitalRead(ENCODER_BTN) == LOW) {
    lcd.display();
    lcd.print("It's hot when");
    lcd.setCursor(0, 1);
    lcd.print(String("it's ") + String(hotTemp,0) + (char)223 + String("C"));
    delay(1000);
    lcd.clear();
    lcd.noDisplay();
    return;
  }

  int newButton = digitalRead(buttonPin);
  if(newButton == HIGH) {
    if (temp >= hotTemp){
      analogWrite(pinR, 255);
    } else if (temp >= warmTemp){
      analogWrite(pinR, 255);
      analogWrite(pinG, 255);
    } else if (temp < warmTemp){
      analogWrite(pinB, 255);
    }
    lcd.display();
    lcd.print(String("It's ") + String(temp,0) + (char)223 + String("C ") + String("H: ") + String(humid,0) + String("%"));
    lcd.setCursor(0, 1);
    lcd.print(String("Feels Like: " + String(feelsLike,0) + (char)223 + String("C)")));
    delay(5000);
    analogWrite(pinR, 0);
    analogWrite(pinG, 0);
    analogWrite(pinB, 0);
    lcd.clear();
    lcd.noDisplay();
  }

  delay(100); // this speeds up the simulation
}