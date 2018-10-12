#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int selectPins[3] = {5, 4, 15};

char auth[] = "";
char ssid[] = "";
char pass[] = "";

int pin_Out_S0 = 5;
int pin_Out_S1 = 4;
int pin_Out_S2 = 15;
int pin_In_Mux1 = A0;
int Mux1_State[8] = {0};
int alarmValue[8] = {0};
int boundery = 0;
int alarmSet= 0;


BlynkTimer timer;

void loop()
{
  timer.run(); // Initiates BlynkTimer
  Blynk.run();
  
}

void setup() {
  
  //mux setup
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT); 
  pinMode(pin_In_Mux1, INPUT);
  
  // Debug console
  Serial.begin(9600);
  Serial.print("Blynk Connecting");
  Blynk.begin(auth, ssid, pass);
  Serial.print("Blynk Connected");

  // Setup a function to be called every two seconds
  timer.setInterval(2000L, timeLoop);
}


void timeLoop(){
  updateValues();
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

BLYNK_WRITE(V20)
  {
     alarmSet = param.asInt();
  }

  
BLYNK_WRITE(V100)
  {
     boundery = param.asInt();
  }

int checkForChange(){
  for (int i = 0; i < 8; i++){
    alarmValue[i]=Mux1_State[i];
  }
}

void selectMuxPin(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}

void updateMux1 () {
  for (int i = 0; i < 8; i++){
    selectMuxPin(i);
    Mux1_State[i] = analogRead(pin_In_Mux1);
  }
}


//This will sample the power 10,000 time and ignore any 0's and set out the avrage.
void updateValues(){
 int x=0;
 int y=0;
 int c=0;
  for (int i = 0; i < 10000; i++){
    y = analogRead(pin_In_Mux1);
    if(y != 0){
      x +=y;
      c++;
    }
  }
  if(x!=0){
    Blynk.virtualWrite(0, x/c);
    Serial.println(x/c); 
  }else{
    Blynk.virtualWrite(0, 0);
    Serial.println(0);
  }
}

//test for for mux 
void updateValues2(){
  updateMux1();
  //Alarm is set
  if(alarmSet == 1){
    checkForChange();    
  }
  for(int i = 0; i < 8; i ++) {
    Blynk.virtualWrite(i, Mux1_State[i]); 
    if(i == 7) {
      Serial.println(Mux1_State[i]);
    } else {
      Serial.print(Mux1_State[i]);
      Serial.print(",");
    }
  }
}
