#include <ESP8266WiFi.h>
#include<Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <virtuabotixRTC.h>
#include<SoftwareSerial.h>

// WIFI CONNECTION
#define FIREBASE_HOST "eco-database-5734f-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Ol8vDLfM4fM7F3F2WD6gmabFZnElM3kGdN5meVZk"
#define WIFI_SSID "GlobeAtHome_123a8_2.4"
#define WIFI_PASSWORD "dv6aZ3Fk"

SoftwareSerial NodeMcu_SoftSerial (D1, D2); // 4, 13
virtuabotixRTC myRTC(D4, D3, D5); //CLK, DATA, RST

char c;
String dataIn;
int8_t indexOfA, indexOfB,indexOfC,indexOfD,indexOfE,indexOfF;

String data1; // M1
String data2; // M2
String data3; // M3
String data4; // NITROGEN
String data5; // POTASSIUM
String data6; // PHOSPHORUS

int sub1 = 15; //D6
int sub2 = 13; //D7
int sub3 = 12; //D8

char state = 0;

byte OffSec = 11; // SECONDS

int sensor1; // M1
int sensor2; // M2
int sensor3; // M3

int Plant1 = 1;
int Plant2 = 2;
int Plant3 = 3;
int All = 4;


void setup() {
  // put your setup code here, to run once:
  pinMode(sub1, OUTPUT);
  pinMode(sub2, OUTPUT);
  pinMode(sub3, OUTPUT);
  Serial.begin(9600);
  
  NodeMcu_SoftSerial.begin(9600);

//Code for RTC
//myRTC.setDS1302Time(00, 24, 00, 6, 4, 6, 2022);
  
// Code for Firebase

 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
  // put your main code here, to run repeatedly:
  int n = 0;

  //Code to get data from firebase
  String Hour1 = Firebase.getString("SetTimeAndDate/hour1");
  String minutes1 = Firebase.getString("SetTimeAndDate/minutes1");
  String day1 = Firebase.getString("SetTimeAndDate/day1");
  String month1 = Firebase.getString("SetTimeAndDate/month1");
  String year1 = Firebase.getString("SetTimeAndDate/year1");
  String choices1 = Firebase.getString("SetTimeAndDate/choices");
  
  Serial.print("Selection: " + choices1);
  Serial.print(" | ");
  Serial.print(Hour1);
  Serial.print(":");
  Serial.print(minutes1);
  Serial.print(" | ");
  Serial.print(day1);
  Serial.print("/");
  Serial.print(month1);
  Serial.print("/");
  Serial.print(year1);
  Serial.println("/");

  int Hour2 = Hour1.toInt();
  int Min2 = minutes1.toInt();
  int day2 = day1.toInt();
  int month2 = month1.toInt();
  int year2 = year1.toInt();
  int choices = choices1.toInt();

  // CODE FOR TIMER
  myRTC.updateTime();

  // Start printing elements as individuals
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  //delay(500);
  if (myRTC.hours == Hour2 && myRTC.minutes == Min2 && myRTC.seconds <= OffSec && myRTC.dayofmonth == day2 && myRTC.month == month2 && myRTC.year == year2 && choices == Plant1)
    {
      Serial.println("Plant1"); 
      digitalWrite(sub1, HIGH);
      digitalWrite(sub2, LOW);
      digitalWrite(sub3, LOW);
      delay(10000);
    }
    else if(myRTC.hours == Hour2 && myRTC.minutes == Min2 && myRTC.seconds <= OffSec && myRTC.dayofmonth == day2 && myRTC.month == month2 && myRTC.year == year2 && choices == Plant2)
    {
      Serial.println("Plant2"); 
      digitalWrite(sub1, LOW);
      digitalWrite(sub2, HIGH);
      digitalWrite(sub3, LOW);
      delay(10000);
    }
    else if(myRTC.hours == Hour2 && myRTC.minutes == Min2 && myRTC.seconds <= OffSec && myRTC.dayofmonth == day2 && myRTC.month == month2 && myRTC.year == year2 && choices == Plant3)
    {
      Serial.println("Plant3"); 
      digitalWrite(sub1, LOW);
      digitalWrite(sub2, LOW);
      digitalWrite(sub3, HIGH);
      delay(10000);
    }
    else if(myRTC.hours == Hour2 && myRTC.minutes == Min2 && myRTC.seconds <= OffSec && myRTC.dayofmonth == day2 && myRTC.month == month2 && myRTC.year == year2 && choices == All)
    {
      Serial.println("SelectAll"); 
      digitalWrite(sub1, HIGH);
      digitalWrite(sub2, HIGH);
      digitalWrite(sub3, HIGH);
      delay(10000);
    }
    else 
    {
      //Serial.println("Hindi Gumagana");
      digitalWrite(sub1, LOW);
      digitalWrite(sub2, LOW);
      digitalWrite(sub3, LOW);
    }

    int sensor1 = data1.toInt(); //Convert data 1 String to int
    int sensor2 = data2.toInt(); //Convert data 2 String to int
    int sensor3 = data3.toInt(); //Convert data 3 String to int
    if (sensor1 < 8){
      //Serial.print(sensor1);
      digitalWrite(sub1, HIGH);
      //delay(50);
      digitalWrite(sub3, LOW);
      digitalWrite(sub2, LOW);
    }  
    else if (sensor2 < 8){
      //Serial.print(sensor2);
      digitalWrite(sub2, HIGH);
      //delay(50);
      digitalWrite(sub1, LOW);
      digitalWrite(sub3, LOW);
    }  
    else if (sensor3 < 8){
      //Serial.print(sensor3);
      digitalWrite(sub3, HIGH);
      //delay(50);
      digitalWrite(sub1, LOW);
      digitalWrite(sub2, LOW);
    }
    else{
      digitalWrite(sub1, LOW);
      digitalWrite(sub2, LOW);
      digitalWrite(sub3, LOW);
    }
   
  // This allows for the update of variables for time or accessing the individual elements.
  
  while(NodeMcu_SoftSerial.available()>0)
  {
    c = NodeMcu_SoftSerial.read();
    
    if(c=='\n')   {break;}
    else          {dataIn+=c;}
  }

  if (c=='\n')
  {
    Parse_the_Data();
    
    Serial.println("M1: " + data1 + "%"); //Prints data to Serial Monitor
    Firebase.setString("Submersible 1", data1); //Prints Data to Firebase
    Serial.println("M2: " + data2 + "%"); //Prints data to Serial Monitor
    Firebase.setString("Submersible 2", data2); //Prints Data to Firebase
    Serial.println("M3: " + data3 + "%"); //Prints data to Serial Monitor
    Firebase.setString("Submersible 3", data3); //Prints Data to Firebase
    Serial.println("N: " + data4 + " mg/kg"); //Prints data to Serial Monitor
    Firebase.setString("Nitrogen", data4 ); //Prints Data to Firebase
    Serial.println("P: " + data5 + " mg/kg"); //Prints data to Serial Monitor
    Firebase.setString("Phosphorus", data5); //Prints Data to Firebase
    Serial.println("K: " + data6 + " mg/kg"); //Prints data to Serial Monitor
    Firebase.setString("Potassium", data6); //Prints Data to Firebase
    Serial.println("==================DATA=======================");
    delay(100);
    c=0;
    dataIn="";
  }
}
void Parse_the_Data()
{
  int indexOfA = dataIn.indexOf("A");
  int indexOfB = dataIn.indexOf("B");
  int indexOfC = dataIn.indexOf("C");
  int indexOfD = dataIn.indexOf("D");
  int indexOfE = dataIn.indexOf("E");
  int indexOfF = dataIn.indexOf("F");

  data1 = dataIn.substring (0, indexOfA);
  data2 = dataIn.substring (indexOfA+1, indexOfB);
  data3 = dataIn.substring (indexOfB+1, indexOfC);
  data4 = dataIn.substring  (indexOfC+1, indexOfD);
  data5 = dataIn.substring  (indexOfD+1, indexOfE);
  data6 = dataIn.substring  (indexOfE+1, indexOfF);
}
