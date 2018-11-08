#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <ArduinoOTA.h>

char auth[] = "";
char ssid[] = "";
char pass[] = "";


#define right_phase 36
#define left_phase 35

BlynkTimer timer;

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

/**
*This is used to disregard the first reading as they have a bleed over from the prior reading.
* pin - This is the pin you would like to prep for reading.
**/
void throw_away(int pin){
  for (int i = 0; i < 700; i++){
    analogRead(pin);
    delay(1);
  }
}

/*
*This is used to check and update the values
*/
void updateValues(){

  int cr=0;
  int cl=0;
  int left_temp=0;
  int right_temp=0;
  int left_avg_value=0;
  int right_avg_value=0;
  int max_value_left=0;
  int max_value_right=0;


  throw_away(left_phase);
  for (int i = 0; i < 15000; i++){

    left_temp = analogRead(left_phase);

    if(left_temp > max_value_left){
      max_value_left = left_temp;
    }

    if(left_temp != 0){
      left_avg_value += left_temp;
      cl++;
    }

  }

  throw_away(right_phase);
  for (int i = 0; i < 15000; i++){

    right_temp = analogRead(right_phase);

    if(right_temp > max_value_right){
      max_value_right = right_temp;
    }

    if(right_temp != 0){
      right_avg_value += right_temp;
      cr++;
    }

  }

  //prevent divide by 0
  if(cl==0){
    cl=1;
  }

  if(cr==0){
    cr=1;
  }

  Serial.print("Left Temp ");
  Serial.print(left_temp);
  Serial.print(" Left Max ");
  Serial.print(max_value_left);
  Serial.print(" Left AVG ");
  Serial.print(left_avg_value /cl);

  Serial.print(" Right Temp ");
  Serial.print(right_temp);
  Serial.print(" Right Max ");
  Serial.print(max_value_right);
  Serial.print(" Right AVG ");
  Serial.print(right_avg_value / cr);
  Serial.println(" ");

  if(left_avg_value!=0){
    Blynk.virtualWrite(1, left_avg_value/cl);
  }

  if(right_avg_value!=0){
    Blynk.virtualWrite(0, right_avg_value/cr);
  }

  Blynk.virtualWrite(21, max_value_left);
  Blynk.virtualWrite(22, max_value_right);

  float div_max_left = max_value_left / 20.00;
  float div_max_right = max_value_right / 20.00;
  Blynk.virtualWrite(23, div_max_left);
  Blynk.virtualWrite(24, div_max_right);
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
  Serial.begin(115200);
  Serial.print("Blynk Connecting");
  Blynk.begin(auth, ssid, pass);
  Serial.print("Blynk Connected");

  // Setup a function to be called every second
  timer.setInterval(1000L, timeLoop);
}


void loop()
{
  timer.run(); // Initiates BlynkTimer
  Blynk.run();

}
