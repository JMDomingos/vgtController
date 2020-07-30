void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  digitalWrite(3, HIGH); // sets the digital pin 13 on
  delay(1000);            // waits for a second
  digitalWrite(3, LOW);  // sets the digital pin 13 off
  delay(1000); 
}
