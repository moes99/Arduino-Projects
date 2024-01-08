int G_LED = 13; // a green led is connected to pin 13
int Y_LED = 9; // a yellow led is connected to pin 9
int R_LED = 5; // a red led is connected to pin 5 
int recheck_button = 3; // to repeat the test.
int time_delay = 2000; //so that any led lights up briefly (for 2 seconds).
float reading, recheck_state, v, g, y, r; // g, y, and r ensure the green, yellow, and red leds respectively only once turn on per test.

void setup() {
pinMode(G_LED, OUTPUT);
pinMode(Y_LED, OUTPUT);
pinMode(R_LED, OUTPUT);
pinMode(recheck_button, INPUT);
pinMode(A0, INPUT);
Serial.begin(9600);
}

void loop() {
reading = analogRead(A0);
recheck_state = digitalRead(recheck_button);
v = 5*reading/1023;
Serial.print("The voltage is: ");
Serial.println(v);

//a reading of 327 corresponds to a voltage of 1.6V.
while(g == 0 && reading >= 327 && reading <= 1023){ 
  digitalWrite(Y_LED, LOW);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, HIGH);
  delay(time_delay);
  digitalWrite(G_LED, LOW);
  g++; 
}

//a reading of 286 corresponds to a voltage of 1.4V.
while(y == 0 && reading >= 286 && reading < 327){ 
  digitalWrite(G_LED, LOW);
  digitalWrite(R_LED, LOW);
  digitalWrite(Y_LED, HIGH);
  delay(time_delay);
  digitalWrite(Y_LED, LOW);
  y++; 
}

while(r == 0 && reading > 0 && reading < 286){
  digitalWrite(Y_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(R_LED, HIGH);
  delay(time_delay);
  digitalWrite(R_LED, LOW);
  r++; 
}
  
if ( g == 1 || y == 1 || r == 1){
  if (recheck_state == HIGH || reading == 0){
    g = 0;
    y = 0;
    r = 0;
  }
}
}
 
