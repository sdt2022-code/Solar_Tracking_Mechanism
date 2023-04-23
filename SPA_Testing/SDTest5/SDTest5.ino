#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// SD //////////////////////////////////////////////////////////////
// Variables
const int chipSelect = 4;
char ReadChar;
char CharArray[35];
int stringindex = 0;
int LineNumber;
int LineCount;
int TargetLine;
int n;
float Nums[6];
File myFile;

// Functions
int GotoTargetLine(int TargetLine);
int ReadNextLine();
void GetNumbers(char InputLine[], int n);
int CountLines();
////////////////////////////////////////////////////////////////////
float TargetAzimuth;
float TargetZenith;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  // Connecting Serial
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  // Initializing SD Card
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  
  TargetLine = 2;
  myFile = SD.open("DataSet.txt");
  LineNumber = GotoTargetLine(TargetLine);
}

void loop() {
  ReadNextLine();
  Serial.println(" ");
  if (!myFile.available()) {
    myFile.close();
  }
  delay(1000);
}

int CountLines() {
  LineCount = 0;
  while (myFile.available()) {
    ReadChar = myFile.read();
    if (ReadChar == '\n') {
      LineCount++;
    }
  }
  return LineCount;
} 

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

int ReadNextLine() {
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
  char InputLine[stringindex];
    for (int i=0; i<stringindex; i++){
      InputLine[i] = CharArray[i];
      Serial.print(InputLine[i]);
      Serial.print(" ");
      Serial.write(InputLine[i]);
    }
  return stringindex;
}


void GetNumbers(char InputLine[], int n) {
  int i = 0;
  int NumIndex = 0;
  String Num = "";
  while (NumIndex < sizeof Nums) {
    while (InputLine[i] != ',') {
      Num += InputLine[i];
      //      Serial.print(i);
      //      Serial.print(" ");
      //      Serial.println(Num);
      i++;
      if (i == n) {
        break;
      }
    }
    Nums[NumIndex] = Num.toFloat();
    Num = "";
    NumIndex++;
    i++;
  }
}
