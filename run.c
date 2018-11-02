#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "";
char ssid[] = "";
char pass[] = "";

int input_pin1 = A0;

BlynkTimer timer;

void loop()
{
  timer.run(); // Initiates BlynkTimer
  Blynk.run();
  
}

void setup() {
  
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.setHostname("PowerMonitor");
  ArduinoOTA.begin();
  
  pinMode(input_pin1, INPUT);
  
  // Debug console
  Serial.begin(9600);
  Serial.print("Blynk Connecting");
  Blynk.begin(auth, ssid, pass);
  Serial.print("Blynk Connected");

  // Setup a function to be called every second
  timer.setInterval(2000L, timeLoop);
}

void timeLoop(){
  updateValues();
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void updateValues(){
 int y=0;
 int c=0;
 int avgValue=0;
 int maxValue=0;
  for (int i = 0; i < 10000; i++){
    y = analogRead(input_pin1);
    if(y > maxValue){
      maxValue = y;
    }
    if(y != 0){
      avgValue +=y;
      c++;
    }
  }
  if(avgValue!=0){
    Blynk.virtualWrite(0, avgValue/c);
    Serial.println(avgValue/c); 
  }else{
    Blynk.virtualWrite(0, 0);
    Serial.println(0);
  }
  
    Blynk.virtualWrite(22, maxValue);
    Serial.println(maxValue);
}
