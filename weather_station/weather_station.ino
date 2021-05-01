#define Type DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

int sensePin = 7, light, dispLCDTime = 0, setTime = 100;
float temp, humidity;
String values;
const int pResistor = 2;

DHT HT(sensePin,Type);
LiquidCrystal LCD(10,9,5,4,3,2);

void setup(){
  Serial.begin(9600);

  LCD.begin(16,2);
  LCD.setCursor(0,0);
  LCD.print("Humidity:");

  LCD.setCursor(0,1);
  LCD.print("Temp:");

  HT.begin();
  delay(setTime);
}

void loop(){
    values= ("Humidity "+get_hum_Value()+','+"Room-Temperature "+get_temp_Value()+","+"Light-Intensity "+get_light_Intensity()+"\n");   // print all sensor values in serial monitor
       delay(1000);
       // removed any buffered previous serial data.
       Serial.flush();
       delay(1000);
       // sent sensors data to serial (sent sensors data to ESP8266)
       Serial.print(values);
       delay(2000);
}

String get_hum_Value(){ 
    LCD.setCursor(9,0);
    LCD.print("      "); 

    delay(setTime);
    humidity = HT.readHumidity();   // get humidity value from DHT111 sensor

    LCD.setCursor(9,0);
    LCD.print(humidity);          // print humidity value on LCD
    delay(dispLCDTime);
    
    return String(humidity);      // return value of humidity to void loop
}

String get_temp_Value(){
    LCD.setCursor(5,1);
    LCD.print("      ");

    delay(setTime);
    temp = HT.readTemperature();    // get room temperature from DHT111 sensor

    LCD.setCursor(5,1);
    LCD.print(temp);         // print room temperature value on LCD
    delay(dispLCDTime);
    
    return String(temp);     // return value of room temperature to void loop
}

String get_light_Intensity(){
    LCD.setCursor(11,1);
    LCD.print("     ");

    delay(setTime);
    light = analogRead(pResistor);

    if(light > 500){
      LCD.setCursor(13,1);
      LCD.print("Day");
      delay(dispLCDTime);
      }
    else{
      LCD.setCursor(11,1);
      LCD.print("Night");
      delay(dispLCDTime);
      }

  return String(light);

}
