//West Side
int West_button = 2;
int GW = 5;
int YW = 4;
int RW = 3;
int bw; //Indicates the state of west_button (HIGH or LOW)

//East Side
int East_button = 10;
int GE = 11;
int YE = 12;
int RE = 13;
int be; //Indicates the state of east_button (HIGH or LOW)

int p = 0;

void setup() {
//Inputs
pinMode(West_button, INPUT);
pinMode(East_button, INPUT);

//OUTPUTS
pinMode(GW, OUTPUT);
pinMode(YW, OUTPUT);
pinMode(RW, OUTPUT);
pinMode(GE, OUTPUT);
pinMode(YE, OUTPUT);
pinMode(RE, OUTPUT);

//Initially or Default
digitalWrite(GW, HIGH);
digitalWrite(RE, HIGH);
}

void loop() {
 bw = digitalRead(West_button);
 be = digitalRead(East_button);
  
 //A car approaches from the east.
 if (be == HIGH && p == 0 && bw == LOW){
  p = 1;
 }
 if (p == 1){
  delay(500);
  digitalWrite(GW, LOW);
  delay(1000);
  digitalWrite(YW, HIGH);
  delay(4000);
  digitalWrite(YW, LOW);
  digitalWrite(RW, HIGH);
  delay(3000);
  digitalWrite(YE, HIGH);
  digitalWrite(RE, LOW);
  delay(3000);
  digitalWrite(GE, HIGH);
  digitalWrite(YE, LOW);
  p = 2;
 }

 //A car approaches from the west.
 if (bw == HIGH && p == 2 && be == LOW){
  p = 3;
 }
 if (p == 3){
  delay(500);
  digitalWrite(GE, LOW);
  delay(1000);
  digitalWrite(YE, HIGH);
  delay(4000);
  digitalWrite(YE, LOW);
  digitalWrite(RE, HIGH);
  delay(3000); 
  digitalWrite(YW, HIGH);
  digitalWrite(RW, LOW);
  delay(3000);
  digitalWrite(GW, HIGH);
  digitalWrite(YW, LOW);
  p = 0;
 }
}
