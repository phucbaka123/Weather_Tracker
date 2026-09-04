#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#define DHTPIN 4
#define DHTTYPE DHT11
#define BUTTONPIN 5
#define REDPIN 18
#define YELLOWPIN 19
#define GREENPIN 23


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16,2);

float minTemp = 1000, maxTemp = -1000;
float minHum = 1000, maxHum = -1000;

bool showMinMax = false;
bool lastbutton = HIGH;
int state = 0; //0 for reg, 1 for min max, 2 for feeling 

// put function declarations here:
int myFunction(int, int);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // set up monitor
  dht.begin(); // setup dht11
  Wire.begin();
  pinMode(BUTTONPIN, INPUT_PULLUP);

  pinMode(REDPIN, OUTPUT);
  pinMode(YELLOWPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);


  lcd.init(); //start lcd 
  lcd.backlight(); //??
  lcd.setCursor(0,0); // set the cursor
  lcd.print("Weather Tracker");
  delay(1500);
  lcd.clear();//clear the "Weather Tracker"

}
void setLight(bool green, bool yellow, bool red){
  digitalWrite(GREENPIN, green);
  digitalWrite(REDPIN, red);
  digitalWrite(YELLOWPIN, yellow);
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
  bool statebutton = digitalRead(BUTTONPIN);
  if( statebutton == LOW && lastbutton == HIGH){
    state = (state + 1) % 3;
    delay(200);
  }
  lastbutton = statebutton;

  //Read the censor every 2s
  static unsigned long lastRead = 0;
  if(millis() - lastRead >= 2000){
    lastRead = millis();

    float humidity = dht.readHumidity();
    float tempC = dht.readTemperature();

    bool tempOk = (tempC >= 21 && tempC <= 30);
    bool humOk = (humidity >= 30 && humidity <= 60);

    if (tempOk && humOk) {
      setLight(true, false, false);   // Green: all good
    } else if (!tempOk && !humOk) {
      setLight(false, false, true);   // Red: both off
    } else {
      setLight(false, true, false);   // Yellow: one off
    }

    if(isnan(humidity) || isnan(tempC)){
      Serial.println("Error! Unable to read the data...");
      return;
    }

    if(humidity > maxHum) maxHum = humidity;
    if(humidity < minHum) minHum = humidity;
    if(tempC > maxTemp) maxTemp = tempC;
    if(tempC < minTemp) minTemp = tempC;

    Serial.print("Temp: "); Serial.print(tempC);
    Serial.print("C | Humidity: "); Serial.print(humidity);
    Serial.println("%");

    if(state == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(tempC);
      lcd.print((char)223);
      lcd.print("C.");

      lcd.setCursor(0,1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.print("% .");


    } else if (state == 1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T:");
      lcd.print(maxTemp);
      lcd.print((char)223);
      lcd.print("C");
      lcd.print(" - ");
      lcd.print(minTemp);
      lcd.print((char)223);
      lcd.print("C.");

      lcd.setCursor(0,1);
      lcd.print("H:");
      lcd.print(maxHum);
      lcd.print("%");
      lcd.print(" - ");
      lcd.print(minHum);
      lcd.print("%.");
    } else if (state == 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("T Fealing:");
      if (tempC < 21){
        lcd.print("Cold");
      } else if (tempC > 30) {
        lcd.print("Hot");
      }
      else{
        lcd.print("Ideal");
      }

      lcd.setCursor(0,1);
      lcd.print("H Fealing:");
      if (humidity < 30){
        lcd.print("Dry");
      } else if (humidity > 60) {
        lcd.print("Humid");
      }
      else{
        lcd.print("Ideal");
      }
    }
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}