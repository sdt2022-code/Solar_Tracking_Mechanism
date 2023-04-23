#include <RTClib.h>
#include <Wire.h> 
#include <SPI.h>
#include <SD.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <AccelStepper.h>
#include <LiquidCrystal.h> 

// RTC and Time-Related Variables and Functions /////////////////////
// Variables 
RTC_DS3231 rtc; 
int s;
int m;
int h;
int D;
int M;
int Y;
int DoY;
int HoY; 


DateTime rtcTime; 
DateTime T0; 
int ElapsedTime;  
const int TimeStep = 60; 
int Nprevious = 0; 
int Ncurrent = 0;

// Functions 
int DayOfYear(int year, int month, int day); 
int HourOfYear(int DoY, int h, int m, int s);
int GetElapsedTime(DateTime T0); 
////////////////////////////////////////////////////////////////////

// Stepper /////////////////////////////////////////////////////////
#define dirPinAz 5
#define stepPinAz 6
#define dirPinZe 2
#define stepPinZe 3
#define motorInterfaceType 1

// Variables  
float TargetAzimuth = 0;
float TargetZenith = 0; 
int AzimuthPosition = 0; 
int ZenithPosition = 0;  
const int GR = 10;
const float StepAngle = 1.8; 
AccelStepper AzimuthStepper = AccelStepper(motorInterfaceType, stepPinAz, dirPinAz);
AccelStepper ZenithStepper = AccelStepper(motorInterfaceType, stepPinZe, dirPinZe);
///////////////////////////////////////////////////////////////////

// LCD /////////////////////////////////////////////////////////
//LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
////////////////////////////////////////////////////////////////

void setup() {
// Initializing RTC
  rtc.begin();       
  rtc.adjust(DateTime(2022, 5, 6, 12, 0, 0));
  T0 = rtc.now();

// Set Stepper Parameters 
  AzimuthStepper.setMaxSpeed(200);
  AzimuthStepper.setAcceleration(30);
  ZenithStepper.setMaxSpeed(200);
  ZenithStepper.setAcceleration(30);
  AzimuthStepper.setCurrentPosition(0); 
  ZenithStepper.setCurrentPosition(0);
  

// Make First Step 
  TargetAzimuth = 20; 
  TargetZenith = 20;
  AzimuthPosition = GR*round(TargetAzimuth/StepAngle); 
  ZenithPosition = GR*round(TargetZenith/StepAngle);   
  AzimuthStepper.moveTo(AzimuthPosition); 
  ZenithStepper.moveTo(ZenithPosition); 
  AzimuthStepper.runToPosition();
  ZenithStepper.runToPosition();
  
  AzimuthStepper.moveTo(0); 
  ZenithStepper.moveTo(0); 
  AzimuthStepper.runToPosition();
  ZenithStepper.runToPosition();
}

void loop() {
  ElapsedTime = GetElapsedTime(T0); 
  Ncurrent = ElapsedTime/TimeStep; 
  if(Ncurrent-Nprevious == 1){
    TargetAzimuth = 20; 
    TargetZenith = 20;
    AzimuthPosition = GR*round(TargetAzimuth/StepAngle); 
    ZenithPosition = GR*round(TargetZenith/StepAngle);  
    AzimuthStepper.moveTo(AzimuthPosition); 
    ZenithStepper.moveTo(ZenithPosition); 
    AzimuthStepper.runToPosition();
    ZenithStepper.runToPosition();   

    AzimuthStepper.moveTo(0); 
    ZenithStepper.moveTo(0); 
    AzimuthStepper.runToPosition();
    ZenithStepper.runToPosition();
   }
  Nprevious = Ncurrent;
  
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("Azimuth: ");
//  lcd.print(AzimuthPosition*StepAngle,2);  
//  lcd.setCursor(0,3); 
//  lcd.print("Zenith: ");
//  lcd.print(ZenithPosition*StepAngle,2); 
  delay(1000);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// RTC and Time-Related Variables and Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetElapsedTime(DateTime T0){
  int ElapsedTime;
  DateTime CurrentTime = rtc.now(); 
  int s = CurrentTime.second();
  int m = CurrentTime.minute();
  int h = CurrentTime.hour(); 
  int s0 = T0.second();
  int m0 = T0.minute();
  int h0 = T0.hour();
  return ElapsedTime = (h-h0)*3600 + (m-m0)*60 + (s-s0); 
  }

int DayOfYear(int year, int month, int day){
    int DoY = day; 
    int days_in_feb = 28; 
    if( (year % 4 == 0 && year % 100 != 0 ) || (year % 400 == 0) )
    {
        days_in_feb = 29;
    }
    switch(month)
    {
        case 2:
            DoY += 31;
            break;
        case 3:
            DoY += 31+days_in_feb;
            break;
        case 4:
            DoY += 31+days_in_feb+31;
            break;
        case 5:
            DoY += 31+days_in_feb+31+30;
            break;
        case 6:
            DoY += 31+days_in_feb+31+30+31;
            break;
        case 7:
            DoY += 31+days_in_feb+31+30+31+30;
            break;            
        case 8:
            DoY += 31+days_in_feb+31+30+31+30+31;
            break;
        case 9:
            DoY += 31+days_in_feb+31+30+31+30+31+31;
            break;
        case 10:
            DoY += 31+days_in_feb+31+30+31+30+31+31+30;            
            break;            
        case 11:
            DoY += 31+days_in_feb+31+30+31+30+31+31+30+31;            
            break;                        
        case 12:
            DoY += 31+days_in_feb+31+30+31+30+31+31+30+31+30;            
            break;                                    
    }
    return DoY; 
}

int HourOfYear(int DoY, int h, int m, int s){
  int HoY; 
  float hoy; 
  h = float(h); 
  m = float(m);
  s = float(s); 
  float t = h + m/60.0 + s/3600.0; ; 
  hoy = 24*(DoY-1) + t; 
  return HoY = round(hoy)+1;   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
