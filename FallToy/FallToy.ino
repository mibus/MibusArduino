/* Copyright (C) 2012, Robert Mibus <mibus@mibus.org>
   Shared under GPLv3 license */
#define xAxis A2
#define yAxis A3
#define zAxis A4

#define ZEROG_PIN 2

#define PIEZO 3

#define RED 9
#define GREEN 10
#define BLUE 11

// 1.65V is 0g.
#define BASE_READING ((1024/5.0)*1.65)
// 0.8V for each G.
#define G_READING ((1024/5.0)*0.8)

int axisValue = 0;

float axisToG(int axis) {
  return (float(axis) - BASE_READING) / G_READING;
}

void setup() {
  Serial.begin(38400);
  Serial.println("Starting up");
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(PIEZO, OUTPUT);  
}  

void loop() {
  if (digitalRead(ZEROG_PIN) == HIGH) {
    // Make sure it's settled...
    delay(20);
    if (digitalRead(ZEROG_PIN) == HIGH) {
      digitalWrite(PIEZO,HIGH);
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      delay(1000);
      return;
    }
  }
  digitalWrite(PIEZO,LOW);
  
  float x,y,z;
  x = axisToG(analogRead(xAxis));
  y = axisToG(analogRead(yAxis));
  z = axisToG(analogRead(zAxis));

  // Something hopefully pretty...
  analogWrite(RED, min(255,max(x,0) * 255));
  analogWrite(GREEN, min(255,max(y,0) * 255));
  analogWrite(BLUE, min(255,max(z,0) * 255));
  
  delay(100);                 
}

