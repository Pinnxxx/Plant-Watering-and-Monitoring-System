#include <SoftwareSerial.h>
#include <Wire.h>
#include "SoftwareSerial.h"
SoftwareSerial Arduino_SoftSerial(13, 4);
// code for NPK
#define RE 8
#define DE 7
const byte code[]  =  {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]  = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[]  = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];
SoftwareSerial mod(2, 3);
////////////////////////////////////////////////////////////////////////
int sensor1_pin = A0;
int sensor2_pin = A1;
int sensor3_pin = A2;
int output1_value;
int output2_value;
int output3_value;
char state = 0;

//String values;
int data1;
int data2;
int data3;
int data4;
int data5;
int data6;
byte val1, val2, val3;
char c;
String dataIn;

void setup() {
  Serial.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  // Sensor Pin and pumps
  //pinMode(sub1, OUTPUT);
  //pinMode(sub2, OUTPUT);
  //pinMode(sub3, OUTPUT);
  pinMode(sensor1_pin, INPUT);
  pinMode(sensor2_pin, INPUT);
  pinMode(sensor3_pin, INPUT);
  Arduino_SoftSerial.begin(9600);
  mod.begin(9600);
  Serial.println("Reading From the Sensor:...");
  delay(500);
}
void loop() {
  nut();
  moisture();
  
  byte val1, val2, val3;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);

  data1 = output1_value;
  data2 = output2_value;
  data3 = output3_value;
  data4 = val1;
  data5 = val2;
  data6 = val3;

  Arduino_SoftSerial.print(data1); Arduino_SoftSerial.print("A");
  Arduino_SoftSerial.print(data2); Arduino_SoftSerial.print("B");
  Arduino_SoftSerial.print(data3); Arduino_SoftSerial.print("C");
  Arduino_SoftSerial.print(data4); Arduino_SoftSerial.print("D");
  Arduino_SoftSerial.print(data5); Arduino_SoftSerial.print("E");
  Arduino_SoftSerial.print(data6); Arduino_SoftSerial.print("F");
  //delay(1000);
  Arduino_SoftSerial.print("\n");
}
void nut() {
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");

  Serial.println("=====================================");
}

byte nitrogen() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(nitro, sizeof(nitro)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();

  }
  return values[4];
}

byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(phos, sizeof(phos)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(pota, sizeof(pota)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(mod.read(),HEX);
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

void moisture()
{
  Serial.print("M1: ");
  Serial.print(output1_value);
  Serial.println("%");
  output1_value = analogRead(sensor1_pin);
  output1_value = map(output1_value, 550, 10, 0, 100);

  Serial.print("M2: ");
  Serial.print(output2_value);
  Serial.println("%");
  output2_value = analogRead(sensor2_pin);
  output2_value = map(output2_value, 550, 10, 0, 100);

  Serial.print("M3: ");
  Serial.print(output3_value);
  Serial.println("%");
  output3_value = analogRead(sensor3_pin);
  output3_value = map(output3_value, 550, 10, 0, 100);
  Serial.println("=====================================");
}
