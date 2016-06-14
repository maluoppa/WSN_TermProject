/*
 LED with light sensor(Light sensor PIN:A2)(LED PIN 13)
 Temperature (and Humidity) sensor with notification (PIN:2)
 Dust sensor with notification (PIN:8)
 */
#include<LAudio.h>
#include "DHT.h"
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;//sampe 30s&nbsp;;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0; 
#define SENSOR_PIN A2
const int ledPin =  13; 
int pin = 8; 
void setup() {
  LAudio.begin();
  LAudio.setVolume(8);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(pin,INPUT);
  starttime = millis();//get the current time;
}
 
void loop() {
  int light = analogRead(SENSOR_PIN);
  Serial.print(light);
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  
  if ((millis()-starttime) >= sampletime_ms)//if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
   }
   /******************************************************/
   if(concentration>=0.5&&light<400){
      LAudio.playFile(storageFlash,(char*)"respirators.mp3");
      Serial.print("Playing\n");
      
      digitalWrite(ledPin, HIGH);
    }else if(concentration>=0.5&&light>=400){
      LAudio.playFile(storageFlash,(char*)"respirators.mp3");
      Serial.print("Playing\n");
      digitalWrite(ledPin, LOW);
    }else if(concentration<0.5&&light<400){
      digitalWrite(ledPin, HIGH);
    }else{
        Serial.print("PAUSE ");
        digitalWrite(ledPin, LOW);
      }
   delay(3000);
   /**********************************************************/
    lowpulseoccupancy = 0;
    starttime = millis();
  if(dht.readHT(&t, &h))
    {
        Serial.println("------------------------------");
        Serial.print("temperature = ");
        Serial.println(t);
        Serial.print("humidity = ");
        Serial.println(h);
    }
   if(t<30)
   {
    LAudio.playFile(storageFlash,(char*)"jacket.mp3");
    Serial.print("Playing jacket");
    if(light<400){
      digitalWrite(ledPin, HIGH);
    }
    else{
      digitalWrite(ledPin, LOW);
    }
    delay(3000);
   }
   
}
