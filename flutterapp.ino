
#include <Arduino.h>
#include <ESP8266WiFi.h> //import for wifi functionality
#include <WebSocketsServer.h> //import for websocket
#include <DHT.h> //library for DHT Sensor

#define dhttype DHT11 //defining DHT Type

const char *ssid =  "HelloHPC - Wifi";   //Wifi SSID (Name)   
const char *pass =  "12345678"; //wifi password

String json; //variable for json 

char tstr[10]; //Character array for temperature 
char hstr[10]; //Character array for Humidity
char istr[10]; //Character array for current
char vstr[10]; //Character array for voltage
char pstr[10]; //Character array for power
char itistr[10]; //Character array for intensity

DHT dht(D5, dhttype); //initialize DHT sensor, D5 is the pin where we connect data pin from sensor

WebSocketsServer webSocket = WebSocketsServer(81); //websocket init with port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//webscket event method
    String cmd = "";
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Websocket is disconnected");
            //case when Websocket is disconnected
            break;
        case WStype_CONNECTED:{
            //wcase when websocket is connected
            Serial.println("Websocket is connected");
            Serial.println(webSocket.remoteIP(num).toString());
            webSocket.sendTXT(num, "connected");}
            break;
        case WStype_TEXT:
            cmd = "";
            for(int i = 0; i < length; i++) {
                cmd = cmd + (char) payload[i]; 
            } //merging payload to single string
            Serial.println(cmd);

            if(cmd == "readdata"){ //when command from app is "poweron"
               //recieved command from mobile app
               //we can do task according to command from mobile using if-else-else if
            } 

             webSocket.sendTXT(num, cmd+":success");
             //send response to mobile, if command is "poweron" then response will be "poweron:success"
             //this response can be used to track down the success of command in mobile app.
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_BIN:
            hexdump(payload, length);
            break;
        default:
            break;
    }
}

String chr2str(char* chr){ //function to convert characters to String
    String rval;
    for(int x = 0; x < strlen(chr); x++){
        rval = rval + chr[x];
    }
    return rval;
}

void setup() {
   Serial.begin(9600); //serial start

   Serial.println("Connecting to wifi");
   
   IPAddress apIP(192, 168, 0, 1);   //Static IP for wifi gateway
   WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //set Static IP gateway on NodeMCU
   WiFi.softAP(ssid, pass); //turn on WIFI

   webSocket.begin(); //websocket Begin
   webSocket.onEvent(webSocketEvent); //set Event for websocket
   Serial.println("Websocket is started");
}

void loop() {
    webSocket.loop(); //keep this line on loop method

    delay(2000); //delay by 2 second, DHT sesor senses data slowly with delay around 2 seconds
    float h = dht.readHumidity(); //Humidity float value from DHT sensor
    float t = dht.readTemperature(); //Temperature float value from DHT sensor
    float f = dht.readTemperature(true); //Temperature float value from DHT sensor

    if (isnan(h) || isnan(t) || isnan(f)) {
        //if data from DHT sensor is null
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }else{
        sprintf(tstr, "%.2f", t); //convert float to characters
        sprintf(hstr, "%.2f", h); //convert float to characters

        json = "{'temp':'" + chr2str(tstr) + "','humidity':'" + chr2str(hstr)'}";
        //formulate JSON string format from characters (Converted to string using chr2str())
        Serial.println("DHT Data read Successful");
        webSocket.broadcastTXT(json); //send JSON to mobile
    }
}