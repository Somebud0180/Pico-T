// Now turn this trash into treasure!

#include <LiquidCrystal.h>
#include <DHT.h>
#include <EasyLed.h>

#define DHTPIN 27
#define DHTTYPE DHT22
#define LEDPIN 26
#define LEDAL HIGH

EasyLed led(LEDPIN, EasyLed::ActiveLevel::Low);

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
DHT dht(DHTPIN, DHTTYPE);
const int buttonPin = 28;
int oldButton = LOW;

void setup() {
  pinMode(buttonPin, INPUT);

  lcd.begin(16, 2);
  lcd.print("Press the button");

  lcd.setCursor(0, 1);
  lcd.print("to check weather");

  delay(5000);
  lcd.clear();
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
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

  int newButton = digitalRead(buttonPin);

  if(newButton != oldButton)
  {
    if(newButton == HIGH)
    {
      led.on();
      lcd.display();
      lcd.print(String("It's ") + String(temp,0) + (char)223 + String("C ") + String("H: ") + String(humid,0) + String("%"));
      lcd.setCursor(0, 1);
      lcd.print(String("Feels Like: " + String(feelsLike,0) + (char)223 + String("C)")));
      delay(5000);
      led.off();
      lcd.clear();
      lcd.noDisplay();
    }
    oldButton = newButton;
  }

  delay(100); // this speeds up the simulation

}