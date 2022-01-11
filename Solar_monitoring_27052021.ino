#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor 

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTTYPE    DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

//////////////Current Sensor///////////////
const int currentPin = A0;
int sensitivity = 100;
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0; double current_pos = 0; 
//////////////////////////////////////////

int sol_v =0; float sol_p=0.0; float solv_a=0;
byte tsv =0; byte tsp=0; byte tsa =0; byte tst=0;
byte tsv_u =0; byte tsv_t =0; byte tsv_h = 0;
int delay_count =0; 
void setup()
{
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(blinkLED);
  Serial.begin (9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  ////////////////////////////////////////
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
  lcd.setCursor(0,0);
	lcd.print("SV=");
  lcd.setCursor(9,0);
  lcd.print("P=");
  lcd.setCursor(0,1);
  lcd.print("SC=");
  lcd.setCursor(9,1);
  lcd.print("T=");
}

void loop()
{
  /////////////Current Sensor////////////////////////
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity)* (-1);
  ///////////////////////////////////////////////////
  

  
  sol_v = analogRead(A1);
  solv_a = sol_v/39.61;
  sol_p = currentValue * solv_a ;
	// Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
  {
    //Serial.println(F("Error reading temperature!"));
  }
  else 
  {
    //Serial.print(F("Temperature: "));
    //Serial.print(event.temperature);
    lcd.setCursor(11,1);
    lcd.println(event.temperature);
    tst =event.temperature;
    
    tsv =solv_a*10; 
    tsp =sol_p ; 
    current_pos =currentValue;
    tsa = current_pos * 10;
    
    lcd.setCursor(3,0);
    lcd.print(solv_a);
    lcd.setCursor(3,1);
    lcd.print(currentValue);
    lcd.setCursor(11,0);
    lcd.print(sol_p);
    //Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    //Serial.println(F("Error reading humidity!"));
  }
  else 
  {
    //Serial.print(F("Humidity: "));
    //Serial.print(event.relative_humidity);
    //Serial.println(F("%"));   
  }
}

void blinkLED(void)
{
  //////////////////////TEMPERATURE/////////////////////////////////
  Serial.print('t');
  Serial.print(tst);
  /////////////////////////////////////////////////////////////////
  
  /////////////////////////////SOLAR AMPERS ///////////////////////
  if (tsa<10)
  {
    Serial.print('c'); Serial.print(0);Serial.print(tsa);  
  } 
  else if (tsa <100 && tsa > 9) 
  {
     Serial.print('c');Serial.print(tsa);
  }
  //////////////////////////////////////////////////////////////////

  ///////////////////////////POWER/////////////////////////////////
   if (tsp<10)
  {
    Serial.print('p'); Serial.print(0);Serial.print(0);Serial.print(tsp);  
  } 
  else if (tsp <100 && tsp > 9) 
  {
     Serial.print('p');Serial.print(0);Serial.print(tsp);
  }
  else if (tsa >100) 
  {
     Serial.print('p');Serial.print(tsp);
  } 
 ////////////////////////////////////////////////////////////////////

 /////////////////////////////voltage////////////////////////////////
  if (tsv<10)
  {
    Serial.print('v'); Serial.print(0);Serial.print(0);Serial.print(tsv);  
  } 
  else if (tsv <100 && tsv > 9) 
  {
     Serial.print('v');Serial.print(0);Serial.print(tsv);
  }
  else if (tsv >100) 
  {
     Serial.print('v');Serial.print(tsv);
  } 


  ///////////////////////////////////////////////////////////////////
  
}
