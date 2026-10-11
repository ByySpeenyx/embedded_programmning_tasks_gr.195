#include <Arduino.h>
uint8_t diodes[] = {4, 5, 6, 7, 8, 9, 10};
uint8_t len = sizeof(diodes)/sizeof(diodes[0]);
int button = 2;
int i = 0;
bool lastReading = HIGH, lastStable = HIGH;
unsigned long lastChange = 0;
bool pressed(){
  bool read = digitalRead(button);
  if(read != lastReading){
    lastReading = read;
    lastChange = millis();
  }
  if(millis() - lastChange > 30){
    lastStable = read;
    if(lastStable == LOW) return true;
    else return false;
  }
}

void time_delay(int b){
  auto time_ = millis();
  while(millis() - time_ < b){ bool read = digitalRead(button); if(read == LOW) break;}
}
void running_fire(){
  for (uint8_t i = 0; i < len ; i++) {
    digitalWrite(diodes[i], HIGH);
    time_delay(1000);
    digitalWrite(diodes[i], LOW);
  }
}

void ongoing_traffic(){
  for(uint8_t i = 0; i < len; i++){
    digitalWrite(diodes[i], HIGH);
    digitalWrite(diodes[len-i-1], HIGH);
    time_delay(1000);
    digitalWrite(diodes[i], LOW);
    digitalWrite(diodes[len-i-1], LOW);
  }
}
void fill_n_empty(){
  for(uint8_t i = 0; i < len; i++){
    digitalWrite(diodes[i], HIGH);
    time_delay(500);
  }
  time_delay(1000);
  for(uint8_t i = 0 ; i < len; i++){
    digitalWrite(diodes[len-i-1], LOW);
  }
}
void (*functions[]) (void) = {running_fire,ongoing_traffic, fill_n_empty};
void setup(){
  for(uint8_t i = 0; i <len; i++) pinMode(diodes[i], OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop(){
  functions[i]();
  if(pressed()){
    i++;
    if(i >= 3) i=0;
  }
}