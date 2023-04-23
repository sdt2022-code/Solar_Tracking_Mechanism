#include <spa.h>
#include "spa.c"


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600); 
    struct Angles SolAngles PROGMEM;
    spa_data spa PROGMEM;  //declare the SPA structure

    struct Time CurrentTime PROGMEM; 
    CurrentTime.year = 2022; 
    CurrentTime.month = 1;
    CurrentTime.day = 1; 
    CurrentTime.hour = 12; 
    CurrentTime.min = 0; 
    CurrentTime.sec = 0;
    CurrentTime.timezone = 1;  
    
    struct Location CurrentLocation PROGMEM; 
    CurrentLocation.latitude = 33.9; 
    CurrentLocation.longitude = 35.5; 
    CurrentLocation.elevation = 0.0; 

    SolAngles = ComputeSunPosition(spa, CurrentTime, CurrentLocation); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
