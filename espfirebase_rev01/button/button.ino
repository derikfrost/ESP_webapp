


#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "addons/TokenHelper.h"

#include "addons/RTDBHelper.h"

#define WIFI_SSID "POCO" // your wifi SSID
#define WIFI_PASSWORD "09091998" //your wifi PASSWORD


#define Relay1 13         //output1
#define Relay2 12         //output2   
#define Relay3 14         //output3
#define Relay4 27         //output4

#define API_KEY "AIzaSyCGyqwUB69LlACJM1myPAWnO3gZI_g1RpE"   // change here
#define DATABASE_URL "https://espbutton-3744e-default-rtdb.firebaseio.com/"  // change here
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;

void setup ()
{
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop ()
{
   if (Firebase.ready() && signupOK ){
  sendDataPrevMillis = millis();
  
  if(Firebase.RTDB.getString(&fbdo, "/Led3Status"))
  {
    String ledstatus = fbdo.stringData();
    if(ledstatus.toInt() == 1){
      Serial.println("on3");
      digitalWrite(Relay3,HIGH);
        
    }
    else {
      Serial.println("off3");
      digitalWrite(Relay3,LOW);  
    }
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(fbdo.errorReason());
  } 
  if(Firebase.RTDB.getString(&fbdo, "/Led1Status")){
    String ledstatus1 = fbdo.stringData();
    if(ledstatus1.toInt() == 1){
      Serial.println("on1");
        digitalWrite(Relay1,HIGH);
    }
    else {
      Serial.println("off1");
        digitalWrite(Relay1,LOW);
    }
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(fbdo.errorReason());
  } 
  if(Firebase.RTDB.getString(&fbdo, "/Led2Status")){
    String ledstatus2 = fbdo.stringData();
    if(ledstatus2.toInt() == 1){
      Serial.println("on2");
        digitalWrite(Relay2,HIGH);
    }
    else {
      Serial.println("off2");
        digitalWrite(Relay2,LOW);
    }
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(fbdo.errorReason());
  } 
  if(Firebase.RTDB.getString(&fbdo, "/Led4Status")){
    String ledstatus3 = fbdo.stringData();
    if(ledstatus3.toInt() == 1){
      Serial.println("on4");
        digitalWrite(Relay4,HIGH);
    }
    else {
      Serial.println("off4");
        digitalWrite(Relay4,LOW);
    }
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(fbdo.errorReason());
  }
   } 
}
