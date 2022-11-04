#define BLYNK_TEMPLATE_ID "TMPLe13ld14G"
#define BLYNK_DEVICE_NAME "Plant Monitoring"
#define BLYNK_AUTH_TOKEN "rudmDnEAFiuqlpAPQH-D7UXmnc39kunw"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>

#define DHTTYPE DHT11
#define sensorPin D3

char auth[] = BLYNK_AUTH_TOKEN;

float h, t;
int mois;
int sensorState = 0;
int pinv0;
//int pinv5;
int count = 0;

DHT dht(D1, DHTTYPE);
BlynkTimer timer;


void sendDHTdata(){
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Oops!!...Failed to read from DHT sensor!");
  }else{
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
    Blynk.virtualWrite(V2, h);
    Blynk.virtualWrite(V3, t);
  
  if(t > 30){
    Blynk.logEvent("temp_alert","Temperature above 30 °C");
  }
  delay(500);
  }
  
}


void moistureSensor(){
  mois = analogRead(A0);
  sensorState = digitalRead(sensorPin);
  Serial.print("Moisture : ");
  Serial.println(mois);
  Serial.print("sensor state : ");
  Serial.println(sensorState);
  Blynk.virtualWrite(V1, mois);
  if(mois > 900){
    Serial.println("water needed for plants");
    Blynk.notify("Water your plants");
    delay(800);
  }else if(mois >= 600 && mois <= 900){
    Serial.println("moisture level is ok");
    Blynk.notify("Moisture level looks fine");
    delay(800);
  }else if(mois >= 480 && mois < 600){
    Serial.println("moisture level is high");
    Blynk.notify("Moisture level is high, doesn't need any water now");
    delay(800);
  }else if(mois < 480){
    Serial.println("excessive moisture found");
    Blynk.notify("Don't water your plants now");
    delay(800);
  }
}

BLYNK_WRITE(V0){
  pinv0 = param.asInt();
  digitalWrite(D2, pinv0);
}

//BLYNK_WRITE(V5){
//  pinv5 = param.asInt();
//}

//void pumpTime(){
//  while(pinv0 == 1){
//    count++;
//    if(count == 50){
//      count = 0;
//      digitalWrite(D2, LOW);
//    }
//  }
//}


void setup()
{
  pinMode(A0,INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(D2,OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, "HBRN7P", "Allowmehere");
  dht.begin();
  timer.setInterval(500L, sendDHTdata);
  timer.setInterval(500L, moistureSensor);
//  timer.setInterval(1000L, pumpTime);
}

void loop() 
{
  Blynk.run();
  timer.run();
}
