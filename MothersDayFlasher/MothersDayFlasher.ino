void setup(){
  for (int i=0;i<5;i++)
    pinMode(i,OUTPUT);
}
int c=0;
void loop(){
  int pin = c % 6;
  int mydelay = c % 24 < 12 ? 100 : 250;
  digitalWrite(pin,0);
  c++;
  pin = c % 6;
  digitalWrite(pin,1);
  delay(mydelay);
}
