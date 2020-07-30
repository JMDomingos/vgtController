int VRPin=2;
volatile unsigned int pulse;
 
void setup() {
  Serial.begin(9600);
 
  pinMode(VRPin, INPUT); //testar INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(VRPin), count_pulse, RISING);
}

void loop() {
  pulse=0;
  interrupts();
  delay(1000);
  noInterrupts();

  Serial.print("Pps: ");
  Serial.println(pulse);

  Serial.print("RPM: ");
  Serial.println(pulse*10); // igual a (x/6) * 60
}

void count_pulse() {
  pulse++;
}
