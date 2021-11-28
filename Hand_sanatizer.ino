#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

// Set these to run example.
#define FIREBASE_HOST "sanitizer-system.firebaseio.com"
#define FIREBASE_AUTH "JH861J3r2Loj2sGBo1IipTMWQ7rnatw4g6suGk7R"
#define WIFI_SSID "MiA3"
#define WIFI_PASSWORD "Shakya1234567"
int Total_counts = 0;
int Trig_hand=D3,Trig_lvl=D6;
int Echo_hand=D2,Echo_lvl=D5;
float waterHeight;
float tankHeight = 14; //enter your water tank height here in [cm]
float totalVolume = 1099.56; // volume here is in [cm^3], change depending on tank volume
float waterQuantity;
float volume;
float pi = 3.14159265359;
float offset = 395.84;
String myString1,myString2,myString3,myString4;
int motor = D7;
void setup() {
  // put your setup code here, to run once:
    pinMode(Trig_hand,OUTPUT);
    pinMode(Echo_hand,INPUT);
    pinMode(Trig_lvl,OUTPUT);
    pinMode(Echo_lvl,INPUT);
    pinMode(motor,OUTPUT);
    Serial.begin(9600);
 // connect to wifi.
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
  Firebase.setString("Sanitizer1/TotalLevelOfsanitizerbottle","sanitizer-system");
  Firebase.setString("Sanitizer1/LevelOfSanitizer","sanitizer-system");
  Firebase.setString("Sanitizer1/AmountOfSanitizerLeft","sanitizer-system"); 
  Firebase.setString("Sanitizer1/TotalNumberOfCountsSanitizerUse","sanitizer-system");
   

}

void loop() {
  // put your main code here, to run repeatedly:
  
  float Hand_detect = Ultrasonic1(Trig_hand,Echo_hand);
  float LVL_detect =  Ultrasonic2(Trig_lvl,Echo_lvl);
  if (Hand_detect<15){
    digitalWrite(motor,HIGH);
    delay(300);
    digitalWrite(motor,LOW);
  
    Total_counts++;
  }
  Serial.print("Total number of counts=");
  Serial.println(Total_counts);
  Serial.print("Level of sanitizer is=");
  Serial.println(LVL_detect);
   myString1= String(tankHeight);
   myString2 = String(LVL_detect);
   myString3 = String(Total_counts);  
  //Serial.println(myString); 
   Firebase.setString("Sanitizer1/TotalLevelOfsanitizerbottle",myString1);
   Firebase.setString("Sanitizer1/LevelOfSanitizer",myString2);
   Firebase.setString("Sanitizer1/TotalNumberOfCountsSanitizerUse",myString3);
  delay(500);


}
float Ultrasonic1(int T,int E){
  digitalWrite(T,LOW);
  delayMicroseconds(2);
  digitalWrite(T,HIGH);
  delayMicroseconds(5);
  digitalWrite(T,LOW);
  int duration=pulseIn(E,HIGH)*0.034/2;
 

 // Serial.println(duration);
  return duration;
  }
  float Ultrasonic2(int T,int E){
  digitalWrite(T,LOW);
  delayMicroseconds(2);
  digitalWrite(T,HIGH);
  delayMicroseconds(5);
  digitalWrite(T,LOW);
  int duration=pulseIn(E,HIGH)*0.034/2;
  volume = pi*16*duration; //change based on the volume for the tank
  waterHeight = tankHeight-duration;
  waterQuantity = totalVolume-volume-offset;
  Serial.print("Water level [cm]: ");
  Serial.println(waterHeight);
  Serial.print("Water Quantity [ml]: ");
  Serial.println(waterQuantity);
  myString4=String(waterQuantity);
  Firebase.setString("Sanitizer1/AmountOfSanitizerLeft",myString4);
  return waterHeight;
  }
