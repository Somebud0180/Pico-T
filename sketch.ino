// Now turn this trash into treasure!

#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 27
#define DHTTYPE DHT22
#define ENCODER_CLK 13
#define ENCODER_DT  14

const int pinR = 20;
const int pinG = 19;
const int pinB = 18;
const int buttonPin = 28;
int oldButton = LOW;
int lastClk = HIGH;
float hotTemp = 32;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial1.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.print("Press the button");

  lcd.setCursor(0, 1);
  lcd.print("to check temps");

  delay(5000);
  lcd.clear();
  dht.begin();
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
    lcd.print(F("Failed to read"));
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor!");
    delay(2000);
    lcd.clear();
    return;
  }

  int newClk = digitalRead(ENCODER_CLK);
  if (newClk != lastClk) {
    // There was a change on the CLK pin
    lastClk = newClk;
    int dtValue = digitalRead(ENCODER_DT);
    if (newClk == LOW && dtValue == HIGH) {
      hotTemp++;
      Serial1.println(hotTemp);
    }
    if (newClk == LOW && dtValue == LOW) {
      hotTemp--;
      Serial1.println(hotTemp);
    }
    float warmTemp = hotTemp - 2;
    float coolTemp = warmTemp - 2;
  }

  int newButton = digitalRead(buttonPin);
  if(newButton != oldButton)
  {
    if(newButton == HIGH)
    {
      lcd.display();
      lcd.print(String("It's ") + String(temp,0) + (char)223 + String("C ") + String("H: ") + String(humid,0) + String("%"));
      lcd.setCursor(0, 1);
      lcd.print(String("Feels Like: " + String(feelsLike,0) + (char)223 + String("C)")));
      delay(5000);
      lcd.clear();
      lcd.noDisplay();
    }
    oldButton = newButton;
  }

  delay(100); // this speeds up the simulation

}