#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>




#define FIREBASE_HOST "solar-1e25e-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "U7V7jDZxfr0CObTOOKEnEfI4slcXxnHqxnuZ4unJ" //Your Firebase Database Secret goes here

#define WIFI_SSID "1234"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "vhvu6169"  
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

byte temp =0; byte voltage =0; byte current =0; byte power=0; byte temperature =0;
float volts = 0.0; float curr = 0.0;

#include <SPI.h>
#include <MFRC522.h>

int user1; int user2; int user3;


//////////////Variable Declaration////////////////////////////

// hundered , tens ,units 
byte t1_u =0; byte t1_t =0; 
byte c1_u =0; byte c1_t =0;
byte p1_u =0; byte p1_t =0; byte p1_h=0;
byte v1_u =0; byte v1_t =0; byte v1_h=0;

///////////////////////////////////////////////////////////////
void setup() 
{
 
  Serial.begin(9600);   // Initiate a serial communication
  
  //////////////firebase code initialization//////// 
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  //Serial.print("Connecting to ");
  //Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
  //////////////////////////////////////////////////
}
void loop() 
{
  //////////////////////String Compare function/////////////
  if (Serial.available() > 0) // if data arrive at serial code
  {
    temp = Serial.read();
    
    
    
    //Serial.print (temp); Serial.print (t1_t);
    
     if (temp == 116)   ///// temperature // decimal degit
     {
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48; //
          t1_t = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          t1_u = temp;
        }
       
       temperature = (t1_t * 10) + t1_u ;  // to make it a double digit number
       Serial.print('t');Serial.println (temperature);
       Firebase.setFloat(firebaseData, "/temperature",temperature);  // transmit data to database
     }
    
     
    else if (temp == 99)   /////current
    {
      while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          c1_t = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          c1_u = temp;
        }
       
       current = (c1_t * 10) + c1_u ;  // to combine
       Serial.print('c');Serial.println (current);
       curr = current /10;
       Firebase.setFloat(firebaseData, "/current",current);
    }
    
    else if (temp== 112)  /////////  Power
    {
      while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          p1_h = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          p1_t = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;
          p1_u = temp;
        }
       
       power = (p1_h * 100) + (p1_t * 10)+ p1_u ;
       Serial.print('p'); Serial.println(power);
       Firebase.setFloat(firebaseData, "/power",power);
    }
    
    else if (temp == 118)  /////////  temperature
    {
       while (Serial.available() == 0);
        {
          temp = Serial.read()-48;  // hundereds
          v1_h = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;  // tens
          v1_t = temp;
        }
        while (Serial.available() == 0);
        {
          temp = Serial.read()-48;   //units
          v1_u = temp;
        }
       
       voltage = (v1_h * 100) + (v1_t * 10)+ v1_u ;
       Serial.print('v'); Serial.println(voltage);
       volts = voltage /10;
       Firebase.setFloat(firebaseData, "/voltage",voltage);
    }
    
    
  } 
    
} 
