#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTPIN 2          // what digital pin we're connected to
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */

#define MUX_A 16
#define MUX_B 5
#define MUX_C 4

#define FIREBASE_HOST "https://dsdp-14455.firebaseio.com/"
#define WLAN_SSID       "Pothik"
#define WLAN_PASS       "mist522515"

void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  delay(5000);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT); 
  dht.begin(); 

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

uint32_t x=0;

void loop() {
  delay(2000);
  float moisture_sensor1;
  changeMux(LOW, LOW, LOW);
  moisture_sensor1 = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
  delay(2000);

  float moisture_sensor2;
  changeMux(LOW, LOW, HIGH);
  moisture_sensor2 = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
  delay(2000);

  Serial.print("Soil Moisture 1 (in Percentage) = ");
  Serial.print(moisture_sensor1);
  Serial.println("%");

   Serial.print(F("\Moisture 1 Val"));
     Serial.print("....");
      if(!Moisture1.publish(moisture_sensor1)){
      Serial.print(F("moisture1 Failed"));
      delay(5000);
      }else{
        Serial.print(F("moisture1 OK"));
        delay(5000);
       }  

   Firebase.setFloat("Moisture1",moisture_sensor1);
    if (Firebase.failed()) {
    Serial.print("Moisture1 failed:");
    Serial.println(Firebase.error()); 
    return;
    }

  delay(5000);

  Serial.print("Soil Moisture 2 (in Percentage) = ");
  Serial.print(moisture_sensor2);
  Serial.println("%");
  delay(5000);

  Serial.print(F("\Moisture 2 Val"));
     Serial.print("....");
      if(!Moisture2.publish(moisture_sensor2)){
      Serial.print(F("moisture2 Failed"));
      delay(5000);
      }else{
        Serial.print(F("moisture2 OK"));
        delay(5000);
       }  

   Firebase.setFloat("Moisture2",moisture_sensor2);
    if (Firebase.failed()) {
    Serial.print("Moisture2 failed:");
    Serial.println(Firebase.error()); 
    return;
    }

  
  changeMux(LOW, HIGH, LOW);
  float voltage = analogRead(sensor_pin) * (5.0 / 1023.0);
  Serial.print(voltage);
  delay(5000);

  Serial.print(F("\Radiation Val"));
    Serial.print("....");
    if(!Radiation.publish(voltage)){
    Serial.print(F("Radiation Failed"));
    delay(5000);
    }else{
      Serial.print(F("Radiation OK"));
      delay(5000);
     }

   Firebase.setFloat("Radiation",voltage);
    if (Firebase.failed()) {
    Serial.print("Radiation failed:");
    Serial.println(Firebase.error()); 
    return;
    }

  

  

  //DHT CODE
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(5000);

    Serial.print(F("\nTemp Val"));
    Serial.print("....");
    if(!Temperature.publish(t)){
    Serial.print(F("Temp Failed"));
    delay(5000);
    }else{
      Serial.print(F("Temp OK"));
      delay(5000);
     }

    Firebase.setFloat("Temperature",t);
    if (Firebase.failed()) {
    Serial.print("Temperature failed:");
    Serial.println(Firebase.error()); 
    return;
    }


     Serial.print(F("\nHUMIDITY Val"));
     Serial.print("....");
     if(!Humidity.publish(h)){
      Serial.print(F("h Failed"));
      delay(5000);
      }else{
        Serial.print(F("h OK"));
        delay(5000);
       }

     Firebase.setFloat("Humidity",h);
     if (Firebase.failed()) {
     Serial.print("Humidity failed:");
     Serial.println(Firebase.error()); 
     return;
     }

  
}
