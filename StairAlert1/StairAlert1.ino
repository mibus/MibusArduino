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

int recent_readings[]={0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0};

int get_recent_average(int *x, int *y, int *z) {
  *x = *y = *z = 0;
  for (int i=0;i<15;i+=3) {
    *x += (recent_readings[i]/5.0);
  }
  for (int i=1;i<15;i+=3) {
    *y += (recent_readings[i]/5.0);    
  }
  for (int i=2;i<15;i+=3) {
    *z += (recent_readings[i]/5.0);
  }
}

void add_recent(int x, int y, int z) {
  for (int i=0;i<3*4;i++) {
    recent_readings[i] = recent_readings[i+3];
  }
  recent_readings[12] = x;
  recent_readings[13] = y;
  recent_readings[14] = z;
}

long last_trigger=0;
long last_safe=0;

void loop() {
  int x, y, z;
  x = analogRead(xAxis);
  y = analogRead(yAxis);
  z = analogRead(zAxis);

  int r_x, r_y, r_z;
  get_recent_average(&r_x, &r_y, &r_z);
  add_recent (x, y, z);

  char foo[] = "1024,1024,1024 vs 1024,1024,1024 (diff of xxxx,xxxx,xxxx=xxxxx)";
  sprintf(foo, "%d,%d,%d vs %d,%d,%d (diff of %d,%d,%d=%d)", x,y,z, r_x, r_y, r_z, abs(x-r_x),abs(y-r_y),abs(z-r_z), abs(x-r_x)+abs(y-r_y)+abs(z-r_z));
  Serial.println(foo);
  Serial.print("Lux: ");
  Serial.println(analogRead(A1));
  

  if (abs(x-r_x)+abs(y-r_y)+abs(z-r_z) >= 45) {
    last_trigger = millis();
  }
  if (analogRead(A1) > 15) {
    delay(10);
    if (analogRead(A1) > 15)
      last_safe = millis();
  }
  int safe=(millis()-last_safe < 5000);
  int triggered=(millis()-last_trigger<2000);
  
  if (safe) {
    digitalWrite(PIEZO,LOW);
    digitalWrite(GREEN,HIGH);
    digitalWrite(RED,LOW);
  } else if (triggered) {
    digitalWrite(PIEZO,HIGH);
    digitalWrite(RED,HIGH);
    digitalWrite(GREEN,LOW);
  } else {
    digitalWrite(PIEZO,LOW);
    digitalWrite(RED,LOW);
    digitalWrite(GREEN,LOW);
  }
  delay(100);
}

