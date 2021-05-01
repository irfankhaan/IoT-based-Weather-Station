#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_AUTH "woJVH3yFvIJKrZBiva2kb74L0Tn3kvEThuJ0867p"   // Enter Firebase API keys/secret here
#define FIREBASE_HOST "weather-station-57841-default-rtdb.firebaseio.com"   // Enter host URL
#define WIFI_SSID "ONEPLUS"   // Enter WiFi SSID
#define WIFI_PASSWORD "J9854321"    // Enter WiFi passwords

const long utcOffsetInSeconds = 19800;    // UTC offset for GMT +5:30
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

String patient_key = "845425413699";    // Random patient key
String sensor_data, light, light_val, hum, temp, hum_val, temp_val;

void setup() {
   Serial.begin(9600);
   
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
    }
  timeClient.begin();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  
  
}
void loop() {
  
  bool Sr =false;
 
  while(Serial.available()){
        sensor_data=Serial.readString();   // Get sensor data from serial put in sensor_data
        Sr=true;          
    }
  
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  String currentDate = String(currentMonth) + "-" + String(monthDay) + "-" + String(currentYear);

  delay(1000);
  
  if(Sr==true){  

  hum = sensor_data.substring(0, sensor_data.indexOf(","));
  hum_val = hum.substring(hum.indexOf(" ")+1);
  sensor_data.remove(0, sensor_data.indexOf(",")+1);
  
  temp = sensor_data.substring(0, sensor_data.indexOf(","));
  temp_val = temp.substring(temp.indexOf(" ")+1);
  sensor_data.remove(0, sensor_data.indexOf(",")+1);
  
  light = sensor_data.substring(0, sensor_data.indexOf("\n"));
  light_val = light.substring(temp.indexOf(" "));

  Firebase.setFloat("/readings/Humidity/"+currentDate+"/"+timeClient.getFormattedTime(),hum_val.toFloat());
  Firebase.setFloat("/readings/Room Temp/"+currentDate+"/"+timeClient.getFormattedTime(),temp_val.toFloat());
  Firebase.setFloat("/readings/Light Intensity/"+currentDate+"/"+timeClient.getFormattedTime(),light_val.toFloat());
  
  delay(1000);
  
  if (Firebase.failed()) {  
      return;
  }
  }   
}
