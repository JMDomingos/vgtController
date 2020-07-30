
int VRPin=2;
int inputBoostPotPin = A0;        // set input pin for the potentiometer
int inputLowBoostPotPin = A1;     // set input pin for the potentiometer

volatile unsigned int pulse;

int inputMaxBoostValue = 0;       // potentiometer input variable
//int solenoidPin = 3;            // set output pin for the solenoid
int dutyCycle = 0;

void setup() {
  Serial.begin(9600);
 
  pinMode(VRPin, INPUT); //INPUT_PULLUP deu sp 0
  attachInterrupt(digitalPinToInterrupt(VRPin), count_pulse, RISING);
  
  // declare the solenoidPin as an OUTPUT:
  //pinMode(solenoidPin, OUTPUT);
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
  
  
  // read the value from the potentiometer:
  inputMaxBoostValue = analogRead(inputBoostPotPin);

  dutyCycle = inputMaxBoostValue/4;
  
  // send the square wave signal to the solenoid:
  //analogWrite(solenoidPin, dutyCycle);
  Serial.print("PWM Duty Cycle: ");
  Serial.println(inputMaxBoostValue/4); //0 a 254 - converter em boost .5 a 1.5
}

void count_pulse() {
  pulse++;
}
