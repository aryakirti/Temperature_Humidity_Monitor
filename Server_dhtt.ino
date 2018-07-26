#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>

#define DHT11_PIN 7

dht DHT;

String apiKey = "93S3XH3E31DE31JH";


SoftwareSerial ser(10, 11); 

void setup() {                  
   pinMode(3,OUTPUT);
   pinMode(2,OUTPUT);

  Serial.begin(115200);
  ser.begin(115200);
  ser.println("AT+RST");
}
void loop() {

 

 
     DHT.read11(DHT11_PIN);   

  
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  if(DHT.temperature>25)
   {
   digitalWrite(3,HIGH);
    digitalWrite(2,LOW); 
   }
   else
   {
   digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
   }

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(DHT.temperature);
  getStr +="&field2=";
  getStr += String(DHT.humidity);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    Serial.println("AT+CIPCLOSE");
  }

  // this delay is required before disabling the ESP8266 chip
  delay(16000);

}
