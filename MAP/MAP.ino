int   inputAnalogMAPSensorPin        = A0;     // set input pin for the MAP sensor
int   inputAnalogMAPSensorValue      = 0;      // MAP sensor input variable

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  Serial.print("MAP(Volts):         ");
  Serial.println(inputAnalogMAPSensorValue);
  delay(500);
}
