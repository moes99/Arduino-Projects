int buzzer = 13;

void setup() {
pinMode(A0, INPUT);
pinMode(buzzer,OUTPUT);
Serial.begin(9600);
analogReference(EXTERNAL);
}

void loop() {
float temperature = analogRead(A0)/5.115;
Serial.print("The temperature is: ");
Serial.println(temperature);
if ( temperature < 26) {
  digitalWrite(buzzer, LOW);
}
if ( temperature >= 26 && temperature < 32){
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
if ( temperature >= 32){
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
}
  
}
