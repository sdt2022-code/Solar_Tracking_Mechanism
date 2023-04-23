#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h> 
LiquidCrystal lcd(3, 2, 4, 5, 6, 7);

//////////////////// Power Measurement Section ////////////////////  
// Measured Voltage, Current, Power, and Battery SOC 
//Timestamp 
int ElapsedTime; 

// Battery 
float VoutBatt; 
//const float BattCorrectionFactor = 1.04978*1.039134; 
const float BattCorrectionFactor = 1; 

float IoutBatt; 
float PoutBatt; 
float SOC; 

// Net Power 
float PNet; 

// Panel  
float VoutPanel; 
const float PanelCorrectionFactor = 1; 
float IoutPanel; 
float PoutPanel; 

// Voltage Sensor Parameters 
float R1 = 30000.0;
float R2 = 7500.0; 

// ACS Current Sensor Parameters 
float VoutACS = 0; 
const double scale_factor20A = 0.1;
const double scale_factor5A = 0.185; 
const double vRef = 4.93;
const double resConvert = 1024;
double resADC = vRef/resConvert;
double zeroPoint = vRef/2;


// Averaging Parameters 
float Sum = 0;                   
const int NumSamples = 10;
//const int SamplingPeriod = 5; // Sampling Period in Minutes 
//const int DelayTime = SamplingPeriod*60*1000;
float FindSOCDischarge(float V); 
//float FindSOCCharge(float V); 

////////////////////////////////////////////////////////////////////

//////////////////// SD Section ////////////////////////////////////  
const int chipSelect = 9;
File dataFile; 
String dataString = "";
////////////////////////////////////////////////////////////////////

void setup() {
    // Initialize LCD Display 
  lcd.begin(20,4);

  // Check for SD Card Initialization 
  lcd.print("Initializing SD card");
  
  if (!SD.begin(chipSelect)) {
    lcd.setCursor(0,1);
    lcd.print("initialization failed!");
    while (1);
  }
  lcd.setCursor(0,1);
  lcd.print("initialization done.");
  delay(5000);
  lcd.clear(); 

  
  // Overwrite Data File 
  SD.remove("datalog.txt");

  // Write Header 
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.print("Timestamp:"); 
  dataFile.print("Solar Output;"); 
  dataFile.print("Battery Output;"); 
  dataFile.println("Net Power Output");
  dataFile.print("V_Panel(V), I_Panel(mA), P_Panel(W);"); 
  dataFile.println(" V_Battery(V), I_Battery(mA), SOC_Battery(%), P_Battery(W); P_Net(W)"); 
  dataFile.close(); 
}

void loop() {
////////////////////////////////////// Panel //////////////////////////////////////////////////////
    // Compute Input Voltage 
    for(int i = 0; i < NumSamples; i++) {
        Sum += analogRead(A4);
        delay(1);
    }
    VoutPanel = ((R1+R2)/R2) * (Sum/NumSamples) * resADC * PanelCorrectionFactor;    
    Sum = 0; 

    // Compute Input Current 
    for(int i = 0; i < NumSamples; i++) {
        Sum += analogRead(A5)*resADC;   
        delay(1);
    }
    VoutACS = Sum/NumSamples; // Calculate from Average
    IoutPanel = (VoutACS - zeroPoint)/scale_factor5A*1000 + 150; // Convert VoutACS into Current using Scale Factor
//    IoutPanel = VoutACS/22*1000; 
    Sum = 0; 
    
    // Compute Input Power 
    PoutPanel = VoutPanel*IoutPanel/1000;  
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////// Battery //////////////////////////////////////////////////////
    // Compute Input Voltage 
    for(int i = 0; i < NumSamples; i++) {
        Sum += analogRead(A2);
        delay(1);
    }
    VoutBatt = ((R1+R2)/R2) * (Sum/NumSamples) * resADC * BattCorrectionFactor;   
    Sum = 0;
    
    // Compute Battery SOC 
    SOC = FindSOCDischarge(VoutBatt);

    // Compute Input Current 
    for(int i = 0; i < NumSamples; i++) {
        Sum += analogRead(A3)*resADC;   
        delay(1);
    }
    VoutACS = Sum/NumSamples; // Calculate from Average
    IoutBatt = (VoutACS - zeroPoint)/scale_factor20A*1000; // Convert VoutACS into Current using Scale Factor
    Sum = 0; 
    
    // Compute Input Power 
    PoutBatt = VoutBatt*IoutBatt/1000; 

    // Compute Net Power 
    PNet = PoutPanel - PoutBatt; 
///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////// SD Section ////////////////////////////////////  
  // Open File for Writing 
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  // Fill data to be written 
  ElapsedTime = millis()/1000; 
  dataString = ""; 
  dataString += String(ElapsedTime);
  dataString += ": "; 
  dataString += String(VoutPanel);
  dataString += ","; 
  dataString += String(IoutPanel);
  dataString += ","; 
  dataString += String(PoutPanel);
  dataString += ";";
  dataString += String(VoutBatt);
  dataString += ","; 
  dataString += String(IoutBatt);
  dataString += ","; 
  dataString += String(SOC); 
  dataString += ","; 
  dataString += String(PoutBatt);
  dataString += ";"; 
  dataString += String(PNet);
  
  // Write and Close 
  dataFile.println(dataString); 
  dataFile.close();
////////////////////////////////////////////////////////////////////
//////////////////// LCD Section ///////////////////////////////////    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("V:");
    lcd.print(VoutPanel,1); 
    lcd.print("V IN "); 
    lcd.print(VoutBatt,1); 
    lcd.print("V OUT");
    lcd.setCursor(0,1);
    lcd.print("I:"); 
    lcd.print(IoutPanel,0); 
    lcd.print("mA IN "); 
    lcd.print(IoutBatt,0); 
    lcd.print("mA OUT");
    lcd.setCursor(0,2);
    lcd.print("P:"); 
    lcd.print(PoutPanel,2);
    lcd.print("W IN "); 
    lcd.print(PoutBatt,2); 
    lcd.print("W OUT");
    lcd.setCursor(0,3); 
    lcd.print("SOC:");
    lcd.print(SOC,0); 
    lcd.print("%"); 
//    lcd.print(PNet);
//    lcd.print("W NET");    

    // Hold for Sampling Period
    delay(5000);
/////////////////////////////////////////////////////////////////////
}

// Function for Calculating Battery SOC from Voltage 
float FindSOCDischarge(float V)
{
    float SOC;
    
    if (V>=12.59)
    {
        SOC = 100;
    }
    
    else if (V>=12.52 && V<12.59)
    {
        SOC = 95+(5*(V-12.52))/(12.59-12.52);
    }  
    else if (V>=12.45 && V<12.52)
    {
        SOC = 90+(5/(12.52-12.45))*(V-12.45);
    } 
      
    else if (V>=12.38 && V<12.45)
    {
        SOC = 85+(5/(12.45-12.38))*(V-12.38);
    }
      
    else if (V>=12.31 && V<12.38)
    {
        SOC = 80+(5/(12.38-12.31))*(V-12.31);
    }
    else if (V>=12.24 && V<12.31)
    {
        SOC = 75+(5/(12.31-12.24))*(V-12.24);
    }
    else if (V>=12.17 && V<12.24)
    {
        SOC = 70+(5/(12.24-12.17))*(V-12.17);
    }
    else if (V>=12.10 && V<12.17)
    {
        SOC = 65+(5/(12.17-12.10))*(V-12.10);
    }
    else if (V>=12.03 && V<12.10)
    {
        SOC = 60+(5/(12.10-12.03))*(V-12.03);
    }
    else if (V>=11.96 && V<12.03)
    {
        SOC = 55+(5/(12.03-11.96))*(V-11.96);
    }
    else if (V>=11.89 && V<11.96)
    {
        SOC = 50+(5/(11.96-11.89))*(V-11.89);
    }
    else if (V>=11.82 && V<11.89)
    {
        SOC = 45+(5/(11.89-11.82))*(V-11.82);
    }
    else if (V>=11.75 && V<11.82)
    {
        SOC = 40+(5/(11.82-11.75))*(V-11.75);
    }
    else if (V>=11.68 && V<11.75)
    {
        SOC = 35+(5/(11.75-11.68))*(V-11.68);
    }
    else if (V>=11.61 && V<11.68)
    {
        SOC = 30+(5/(11.68-11.61))*(V-11.61);
    }
    else if (V>=11.54 && V<11.61)
    {
        SOC = 25+(5/(11.61-11.54))*(V-11.54);
    }
    else if (V>=11.47 && V<11.54)
    {
        SOC = 20+(5/(11.54-11.47))*(V-11.47);
    }
    else if (V>=11.40 && V<11.47)
    {
        SOC = 15+(5/(11.47-11.40))*(V-11.40);
    }
    else if (V>=11.33 && V<11.40)
    {
        SOC = 10+(5/(11.40-11.33))*(V-11.33);
    }
    else if (V<11.33)
    {
        SOC = 0;
    }
  return SOC; 
}

//
//float FindSOCCharge(float V)
//{
//    float SOC;
//    
//    if (V>=13.78)
//    {
//        SOC = 100;
//    }
//    
//    else if (V>=13.71 && V<13.78)
//    {
//        SOC = 95+(5*(V-13.71))/(13.78-13.71);
//    }  
//
//    else if (V>=13.64 && V<13.71)
//    {
//        SOC = 90+(5/(13.71-13.64))*(V-13.64);
//    } 
//      
//    else if (V>=13.57 && V<13.64)
//    {
//        SOC = 85+(5/(13.64-13.57))*(V-13.57);
//    }
//      
//    else if (V>=13.50 && V<13.57)
//    {
//        SOC = 80+(5/(13.57-13.50))*(V-13.50);
//    }
//    else if (V>=13.43 && V<13.50)
//    {
//        SOC = 75+(5/(13.50-13.43))*(V-13.43);
//    }
//    else if (V>=13.36 && V<13.43)
//    {
//        SOC = 70+(5/(13.43-13.36))*(V-13.36);
//    }
//    else if (V>=13.29 && V<13.36)
//    {
//        SOC = 65+(5/(13.36-13.29))*(V-13.29);
//    }
//    else if (V>=13.22 && V<13.29)
//    {
//        SOC = 60+(5/(13.29-13.22))*(V-13.22);
//    }
//    else if (V>=13.15 && V<13.22)
//    {
//        SOC = 55+(5/(13.22-13.15))*(V-13.15);
//    }
//    else if (V>=13.08 && V<13.15)
//    {
//        SOC = 50+(5/(13.15-13.08))*(V-13.08);
//    }
//    else if (V>=13.01 && V<13.08)
//    {
//        SOC = 45+(5/(13.08-13.01))*(V-13.01);
//    }
//    else if (V>=12.94 && V<13.01)
//    {
//        SOC = 40+(5/(13.01-12.94))*(V-12.94);
//    }
//    else if (V>=12.87 && V<12.94)
//    {
//        SOC = 35+(5/(12.94-12.87))*(V-12.87);
//    }
//    else if (V>=12.80 && V<12.87)
//    {
//        SOC = 30+(5/(12.87-12.80))*(V-12.80);
//    }
//    else if (V>=12.73 && V<12.80)
//    {
//        SOC = 25+(5/(12.80-12.73))*(V-12.73);
//    }
//    else if (V>=12.66 && V<12.73)
//    {
//        SOC = 20+(5/(12.73-12.66))*(V-12.66);
//    }
//    else if (V>=12.60 && V<12.66)
//    {
//        SOC = 15+(5/(12.66-11.60))*(V-12.60);
//    }
//    else if (V>=12.54 && V<12.60)
//    {
//        SOC = 10+(5/(12.60-12.54))*(V-12.54);
//    }
//    else if (V<12.54)
//    {
//        SOC = 0;
//    }
//  return SOC; 
//}
