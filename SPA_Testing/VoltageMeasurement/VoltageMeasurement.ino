//#include <LiquidCrystal.h> 
//LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

float I; 
float DeltaV; 
float V1; 
float V2; 
float Rshunt = 100.0; 
float R1 = 100; 
float R2 = 10; 
float ConvFact = R2/(R1+R2)*(5/1024);
float Sum1 = 0;                    
float Sum2 = 0;  
const int NumSamples = 100;

void setup()
{
  Serial.begin(9600); 
  Serial.println("Values: ");
//    lcd.begin(20,2);
}


void loop()
{
    for(int i = 0; i < NumSamples; i++) {
        Sum1 += analogRead(A4);
        delay(1);
    }
    for(int i = 0; i < NumSamples; i++) {
        Sum2 += analogRead(A5);
        delay(1);
    }
    V1 = Sum1*ConvFact; 
    V2 = Sum2*ConvFact;     
    DeltaV = V2-V1; 
    I = DeltaV/Rshunt; 
    Sum1 = 0;                    
    Sum2 = 0;  
    Serial.println(V1); 
    Serial.println(V2); 
    Serial.println(I); 
    Serial.println(" ");
//    lcd.clear(); 
//    lcd.setCursor(0,1);
//    lcd.print("I=");
//    lcd.print(I); 
//    lcd.print(" A");  
}
