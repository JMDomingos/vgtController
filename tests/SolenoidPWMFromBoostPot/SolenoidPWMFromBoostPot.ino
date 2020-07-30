int inputBoostPotPin = A0;  // set input pin for the potentiometer
int inputMaxBoostValue = 0; // potentiometer input variable
int solenoidPin = 3;        // set output pin for the solenoid
int dutyCycle = 0;

void setup() {
  // declare the solenoidPin as an OUTPUT:
  pinMode(solenoidPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // read the value from the potentiometer:
  inputMaxBoostValue = analogRead(inputBoostPotPin);

  dutyCycle = inputMaxBoostValue/4;
  
  // send the square wave signal to the solenoid:
  analogWrite(solenoidPin, dutyCycle);

  Serial.print("PWM Duty Cycle: ");
  Serial.println(inputMaxBoostValue/4);
}
