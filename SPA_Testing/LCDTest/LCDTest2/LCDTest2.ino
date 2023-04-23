#include <LiquidCrystal.h> 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

float R1 = 100; 
float R2 = 10; 
float sum = 0;                    // sum of samples taken
const int NumSamples = 100;
unsigned char SampleCount = 0; // current sample number
float Vin;            // calculated voltage
float SOC;

void setup()
{
    lcd.begin(16,2);
//    Serial.begin(9600);
}

float FindSOC(float V)
{
    float SOC;
    
    if (V>=12.73)
    {
        SOC = 100;
    }
    
    else if (V>=12.62 && V<12.73)
    {
        SOC = 90+(10*(V-12.62))/(12.73-12.62);
    }  

    else if (V>=12.5 && V<12.62)
    {
        SOC = 80+10/(12.62-12.5)*(V-12.5);
    } 
      
    else if (V>=12.37 && V<12.5)
    {
        SOC = 70+10/(12.5-12.37)*(V-12.37);
    }
      
    else if (V>=12.24 && V<12.37)
    {
        SOC = 60+10/(12.37-12.24)*(V-12.24);
    }
    else if (V>=12.10 && V<12.24)
    {
        SOC = 50+10/(12.24-12.10)*(V-12.10);
    }
    else if (V>=11.96 && V<12.10)
    {
        SOC = 40+10/(12.10-11.96)*(V-11.96);
    }
    else if (V>=11.81 && V<11.96)
    {
        SOC = 30+10/(11.96-11.81)*(V-11.81);
    }
    else if (V>=11.66 && V<11.81)
    {
        SOC = 20+10/(11.81-11.66)*(V-11.66);
    }
    else if (V>=11.51 && V<11.66)
    {
        SOC = 10+10/(11.66-11.51)*(V-11.51);
    }
    else if (V<11.51)
    {
        SOC = 0;
    }
  return SOC; 
}

void loop()
{
    while (SampleCount < NumSamples) {
        sum += analogRead(A0);
        SampleCount++;
        delay(10);
    }
    Vin = ((R1+R2)/R2) * (sum/NumSamples * 5) / 1024.0;
    SOC = FindSOC(Vin); 
    lcd.setCursor(0,0);
    lcd.print("Vin=");
    lcd.print(Vin); 
    lcd.print("V");  
    lcd.setCursor(0,1);
    lcd.print("SOC="); 
    lcd.print(SOC);
    lcd.print("%");
//    Serial.println("%"); 
    SampleCount = 0;
    sum = 0;
    delay(4000);
    lcd.clear(); 
}
