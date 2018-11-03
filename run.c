#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <ArduinoOTA.h>

char auth[] = "";
char ssid[] = "";
char pass[] = "";



int right_phase = A0;
int left_phase = A3;

BlynkTimer timer;

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void updateValues(){

  int cr=0;
  int cl=0;
  int left_temp=0;
  int right_temp=0;
  int left_avg_value=0;
  int right_avg_value=0;
  int max_value_left=0;
  int max_value_right=0;

  for (int i = 0; i < 5000; i++){

    left_temp = analogRead(right_phase);
    right_temp = analogRead(right_phase);

    if(left_temp > max_value_left){
      max_value_left = left_temp;
    }

    if(right_temp > max_value_right){
      max_value_right = right_temp;
    }

    if(left_temp != 0){
      left_avg_value += left_temp;
      cl++;
    }

    if(right_temp != 0){
      right_avg_value += right_temp;
      cr++;
    }

  }

  //Serial.print("avg_value ");
  if(left_avg_value!=0){
    Blynk.virtualWrite(1, left_avg_value/cl);
    //Serial.println(avg_value/cl);
  }else{
    Blynk.virtualWrite(1, 0);
    //Serial.println(0);
  }

  //Serial.print("avg_value ");
  if(right_avg_value!=0){
    Blynk.virtualWrite(0, right_avg_value/cr);
    //Serial.println(avg_value/cr);
  }else{
    Blynk.virtualWrite(0, 0);
    //Serial.println(0);
  }

  Blynk.virtualWrite(21, max_value_left);
  Blynk.virtualWrite(22, max_value_right);

  float div_max_left = max_value_left / 20.00;
  float div_max_right = max_value_right / 20.00;
  Blynk.virtualWrite(23, div_max_left);
  Blynk.virtualWrite(24, div_max_right);

  //Serial.print("max value right ");
  //Serial.println(max_value_right);
  //Serial.print("max_value_right  / 20. ");
  //Serial.println(divMax);
}

void timeLoop(){
  updateValues();
}


void setup() {

  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.setHostname("PowerMonitor");
  ArduinoOTA.begin();

  pinMode(right_phase, INPUT);
  pinMode(left_phase, INPUT);

  // Debug console
  Serial.begin(9600);
  Serial.print("Blynk Connecting");
  Blynk.begin(auth, ssid, pass);
  Serial.print("Blynk Connected");

  // Setup a function to be called every second
  timer.setInterval(2500L, timeLoop);
}


void loop()
{
  timer.run(); // Initiates BlynkTimer
  Blynk.run();

}
