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
//String CurrentDate; 
//String CurrentTime;


DateTime rtcTime; 
DateTime T0; 
int ElapsedTime;  
const int TimeStep = 5; 
int Nprevious = 0; 
int Ncurrent = 0;

// Functions 
int DayOfYear(int year, int month, int day); 
int HourOfYear(int DoY, int h, int m, int s);
int GetElapsedTime(DateTime T0); 
////////////////////////////////////////////////////////////////////

// SD //////////////////////////////////////////////////////////////
// Variables
const int chipSelect = 4;
char ReadChar;
char CharArray[35];
char Num[10];
int stringindex = 0;
int LineNumber;
int TargetLine;
float Nums[6];
File myFile;

// Functions
int GotoTargetLine(int TargetLine);
void ReadNextLine();
void GetNumbers(char InputLine[], int n);
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
const float AzimuthPositions[3] = {95.94,106.80,123.31}; 
const float ZenithPositions[3] = {50.92,38.71,27.40};
int j = 0; 
const int GR = 10;
const float StepAngle = 1.8; 
AccelStepper AzimuthStepper = AccelStepper(motorInterfaceType, stepPinAz, dirPinAz);
AccelStepper ZenithStepper = AccelStepper(motorInterfaceType, stepPinZe, dirPinZe);
///////////////////////////////////////////////////////////////////

// LCD /////////////////////////////////////////////////////////
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
////////////////////////////////////////////////////////////////

void setup() {
// Initializing RTC
  rtc.begin();       
  rtc.adjust(DateTime(2022, 5, 6, 9, 0, 0));

// Initializing LCD 
  lcd.begin(20,4);
  
// Initializing SD Card 
//  Serial.print("Initializing SD card...");
//  if (!SD.begin(chipSelect)) {
//    Serial.println("initialization failed!");
//    while (1);
//  }
//  Serial.println("initialization done."); 
// Set Stepper Parameters 
  AzimuthStepper.setMaxSpeed(200);
  AzimuthStepper.setAcceleration(30);
  ZenithStepper.setMaxSpeed(200);
  ZenithStepper.setAcceleration(30);
  AzimuthStepper.setCurrentPosition(0); 
  ZenithStepper.setCurrentPosition(0);

  delay(3000); 
    TargetAzimuth = AzimuthPositions[j]*GR; 
    TargetZenith = ZenithPositions[j]*GR; 
    AzimuthPosition = round(TargetAzimuth/StepAngle); 
    ZenithPosition = round(TargetZenith/StepAngle);  
    AzimuthStepper.moveTo(AzimuthPosition); 
    ZenithStepper.moveTo(ZenithPosition); 
    AzimuthStepper.runToPosition();
    ZenithStepper.runToPosition();   
    j++;
    
  DateTime rtcTime = rtc.now();  
  s = rtcTime.second();
  m = rtcTime.minute();
  h = rtcTime.hour();
  D = rtcTime.day();
  M = rtcTime.month();
  Y = rtcTime.year();  
//  CurrentDate = String(D)+"/"+String(M)+"/"+String(Y); 
//  CurrentTime = String(h)+":"+String(m)+":"+String(s);;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Day:     "); 
//  lcd.print(CurrentDate); 
  if (D < 10) lcd.print("0");
  lcd.print(D);
  lcd.print("/"); 
  if (M < 10) lcd.print("0");
  lcd.print(M);
  lcd.print("/"); 
  lcd.print(Y);
  lcd.setCursor(0,1);
  lcd.print("Time:    ");
//  lcd.print(CurrentTime);
  if (h < 10) lcd.print("0");
  lcd.print(h);
  lcd.print(":");
  if (m < 10) lcd.print("0"); 
  lcd.print(m);
  lcd.print(":"); 
  if (s < 10) lcd.print("0"); 
  lcd.print(s); 
  lcd.setCursor(0,2);
  lcd.print("Azimuth: ");
  lcd.print(float(AzimuthPosition)/float(GR)*StepAngle,2);  
  lcd.setCursor(0,3); 
  lcd.print("Zenith:  ");
  lcd.print(float(ZenithPosition)/float(GR)*StepAngle,2);
  
  T0 = rtc.now();
// Open File and Prepare for Reading   
//  myFile = SD.open("DataSet.txt");
//  LineNumber = GotoTargetLine(HoY);
//
//  for(int j=0;j<3;j++){
//    ReadNextLine(); 
//    TargetAzimuth = Nums[4]; 
//    TargetZenith = Nums[5];
//    AzimuthPositions[j] = TargetAzimuth;
//    ZenithPositions[j] = TargetZenith;
//    }
//  myFile.close(); 
// Read First Line 
//  ReadNextLine(); 
//  TargetAzimuth = Nums[4]; 
//  TargetZenith = Nums[5];

//  
//// Make First Step 
//  TargetAzimuth = 20; 
//  TargetZenith = 20;
//  AzimuthPosition = GR*round(TargetAzimuth/StepAngle); 
//  ZenithPosition = GR*round(TargetZenith/StepAngle);   
//  AzimuthStepper.moveTo(AzimuthPosition); 
//  ZenithStepper.moveTo(ZenithPosition); 
//  AzimuthStepper.runToPosition();
//  ZenithStepper.runToPosition();
}

void loop() {
  delay(3000); 
    TargetAzimuth = GR*AzimuthPositions[j]; 
    TargetZenith = GR*ZenithPositions[j]; 
    AzimuthPosition = round(TargetAzimuth/StepAngle); 
    ZenithPosition =  round(TargetZenith/StepAngle);  
    AzimuthStepper.moveTo(AzimuthPosition); 
    ZenithStepper.moveTo(ZenithPosition); 
    AzimuthStepper.runToPosition();
    ZenithStepper.runToPosition();   
    j++; 
  
  DateTime rtcTime = rtc.now();  
  s = rtcTime.second();
  m = rtcTime.minute();
  h = rtcTime.hour();
  D = rtcTime.day();
  M = rtcTime.month();
  Y = rtcTime.year(); 
   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Day:     "); 
//  lcd.print(CurrentDate); 
  if (D < 10) lcd.print("0");
  lcd.print(D);
  lcd.print("/"); 
  if (M < 10) lcd.print("0");
  lcd.print(M);
  lcd.print("/"); 
  lcd.print(Y);
  lcd.setCursor(0,1);
  lcd.print("Time:    ");
//  lcd.print(CurrentTime);
  if (h < 10) lcd.print("0");
  lcd.print(h);
  lcd.print(":");
  if (m < 10) lcd.print("0"); 
  lcd.print(m);
  lcd.print(":"); 
  if (s < 10) lcd.print("0"); 
  lcd.print(s); 
  lcd.setCursor(0,2);
  lcd.print("Azimuth: ");
  lcd.print(float(AzimuthPosition)/float(GR)*StepAngle,2);  
  lcd.setCursor(0,3); 
  lcd.print("Zenith:  ");
  lcd.print(float(ZenithPosition)/float(GR)*StepAngle,2);
//  CurrentDate = String(D)+"/"+String(M)+"/"+String(Y); 
//  CurrentTime = String(h)+":"+String(m)+":"+String(s);;
 
//  ElapsedTime = GetElapsedTime(T0); 
////  Ncurrent = ElapsedTime/TimeStep; 
////  if(Ncurrent-Nprevious == 1)
//    if(ElapsedTime == TimeStep){
////    ReadNextLine(); 
////    TargetAzimuth = Nums[4]; 
////    TargetZenith = Nums[5];
//    TargetAzimuth = GR*AzimuthPositions[j]; 
//    TargetZenith = GR*ZenithPositions[j]; 
//    AzimuthPosition = round(TargetAzimuth/StepAngle); 
//    ZenithPosition =  round(TargetZenith/StepAngle);  
//    AzimuthStepper.moveTo(AzimuthPosition); 
//    ZenithStepper.moveTo(ZenithPosition); 
//    AzimuthStepper.runToPosition();
//    ZenithStepper.runToPosition();   
//    j++; 
//    T0 = rtc.now(); 
//   }
//  Nprevious = Ncurrent;


  if(j == 3){
    delay(3000); 
    AzimuthStepper.moveTo(0); 
    ZenithStepper.moveTo(0); 
    AzimuthStepper.runToPosition();
    ZenithStepper.runToPosition();  
  DateTime rtcTime = rtc.now();  
  s = rtcTime.second();
  m = rtcTime.minute();
  h = rtcTime.hour();
  D = rtcTime.day();
  M = rtcTime.month();
  Y = rtcTime.year(); 
   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Day:     "); 
//  lcd.print(CurrentDate); 
  if (D < 10) lcd.print("0");
  lcd.print(D);
  lcd.print("/"); 
  if (M < 10) lcd.print("0");
  lcd.print(M);
  lcd.print("/"); 
  lcd.print(Y);
  lcd.setCursor(0,1);
  lcd.print("Time:    ");
//  lcd.print(CurrentTime);
  if (h < 10) lcd.print("0");
  lcd.print(h);
  lcd.print(":");
  if (m < 10) lcd.print("0"); 
  lcd.print(m);
  lcd.print(":"); 
  if (s < 10) lcd.print("0"); 
  lcd.print(s); 
  lcd.setCursor(0,2);
  lcd.print("Azimuth: ");
  lcd.print(float(AzimuthPosition)/float(GR)*StepAngle,2);  
  lcd.setCursor(0,3); 
  lcd.print("Zenith:  ");
  lcd.print(float(ZenithPosition)/float(GR)*StepAngle,2);
    while(true); 
    }  
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

// SD ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GotoTargetLine(int TargetLine) {
  int LineNumber = 1;
  while (myFile.available()) {
    if (myFile.read() == '\n') {
      LineNumber++;
    }
    if (LineNumber == TargetLine) {
      break;
    }
  }
  return LineNumber;
}

void ReadNextLine() {
  stringindex = 0;
  while (myFile.available()) {
    ReadChar = myFile.read();
    if (ReadChar != '\n') {
      CharArray[stringindex] = ReadChar;
      stringindex++;
    }
    if (ReadChar == '\n') {
      break;
    }
  }
  GetNumbers(CharArray, stringindex);
  return stringindex;
}


void GetNumbers(char InputLine[], int n) {
  int i = 0;
  int j = 0; 
  int NumIndex = 0;
  while (NumIndex < 6) {
    while (InputLine[i] != ',') {
      Num[j] = InputLine[i];
//            Serial.print(i);
//            Serial.print(" ");
//            Serial.print(j);
//            Serial.print(" ");
//            Serial.println(Num);
      i++;
      j++;
      if (i == n) {
        break;
      }
    }
    Nums[NumIndex] = atof(Num);
    NumIndex++;
    i++;
    j = 0; 
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
