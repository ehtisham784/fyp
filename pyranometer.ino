#define ANALOG_PIN A0 // Analog pin
#define RESISTANCE 10 // Resistance in thousands of ohms
#define PANEL_LENGTH 600 // Length of solar cell in mm
#define PANEL_WIDTH 200 // Width of solar cell in mm
volatile float Area;
volatile float Power;
volatile float Radiation;
/*
* Main Setup function
*/
void setup() {
// Begin serial communication
Serial.begin(9600);
while(!Serial);
delay(100);
}
/*
* Main Setup function
*/
void loop() {
Area = PANEL_LENGTH * PANEL_WIDTH / (1000*1000); // we are dividing by 1000000 get the area in square meters
Power = pow(analogRead( ANALOG_PIN ), 2) / RESISTANCE ; // Calculating power
Radiation = Power / Area;
char *msg; 
sprintf(msg, " The Solar Radiation is %f W/M2 ", Radiation); // Generating message to be printed 
Serial.println(msg);
delay(1000);
}