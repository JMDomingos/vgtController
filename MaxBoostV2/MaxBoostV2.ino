int   inputMaxBoostPin                = A0;   // set input pin for the potentiometer
int   inputVRSensorPin                = A2;   // set input pin for the VR sensor
int   inputMAPSensorPin               = A3;   // set input pin for the MAP sensor

int   solenoidPin                     = 3;    // set output pin for the solenoid

int   dutyCycle                       = 225;
int   dutyCycleSteps                  = 1;    // Instead of increasing 1 by 1
int   minDutyCycle                    = 115;   
int   maxDutyCycle                    = 245;  

int   inputMaxBoostValue              = 0;    // potentiometer input variable
float MaxBoostValue                   = 0.50; // potentiometer value store
float MaxBoostValueLimit              = 1.70; // maximum value allowed (will be 1.70)

float startupMAPReading               = 0.00; // MAP reading at startup to store zero bar value
float MAPSensorValue                  = 0.00;

int   inputVRSensorValue              = 0;    // VR sensor input variable
int   RPMSensorValue                  = 0;

int   maxDutyCyclePerRPMVolts[5]      = { };  // RPM Reference table
int   maxDutyCyclePerRPM[5]           = { };  // RPM Reference table

void setup() {
  // Fill maxDutyCycle p/ RPM Array - TODO: Put right values and make maps for right pot
  maxDutyCyclePerRPMVolts[0] = 580;      maxDutyCyclePerRPM[0] = maxDutyCycle;
  maxDutyCyclePerRPMVolts[1] = 650;      maxDutyCyclePerRPM[1] = 245;  // 1200 RPM
  maxDutyCyclePerRPMVolts[2] = 765;      maxDutyCyclePerRPM[2] = 240;  // 1500 RPM
  maxDutyCyclePerRPMVolts[3] = 920;      maxDutyCyclePerRPM[4] = 235;  // 2000 RPM 
  maxDutyCyclePerRPMVolts[4] = 1020;     maxDutyCyclePerRPM[5] = 230;  // 2400 RPM 
  
  pinMode(solenoidPin, OUTPUT);               // declare the solenoidPin as an OUTPUT:
  
  startupMAPReading = analogRead(inputMAPSensorPin);
  
  Serial.begin(9600);
}

void loop() {
  inputMaxBoostValue = analogRead(inputMaxBoostPin);  //Gets maxBoostValue from pot
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;

  for(int y=0;y<100;y++) {   // Correr aqui dentro para evitar fazer td constantemente
    // fetch RPM
    RPMSensorValue = fetchRPM();

    // boost comtrols
    if(RPMSensorValue < 580) {                            // Below 1000 RPM
      if(dutyCycle > minDutyCycle) {
        stopDutyCycle();
        Serial.println("Below 1000 - After stopping dutyCycle"); //APAGAR DPS
      }
    }

    delay(100);
  }
}

float findBoostFromVoltage(int voltage) {
  int tempVoltage = (voltage - startupMAPReading);
  float boost = (tempVoltage / 400.00);
  return boost;
}

float fetchMAP() {
  int inputMAPSensorValue = 0;
  inputMAPSensorValue = analogRead(inputMAPSensorPin);    // fetch MAP
  return findBoostFromVoltage(inputMAPSensorValue);
}

int fetchRPM() {
  // 0 RPM    = 305 Volts;
  // 600 RPM  = 410 Volts;
  // 700 RPM  = 450 Volts;
  // 800 RPM  = 490 Volts;
  // 900 RPM  = 510 Volts;
  // 1000 RPM = 580 Volts;
  // 1100 RPM = 615 Volts;
  // 1200 RPM = 650 Volts;
  // 1300 RPM = 685 Volts;
  // 1400 RPM = 725 Volts;
  // 1500 RPM = 765 Volts;
  // 2000 RPM = 920 Volts;
  // 2400 RPM = 1023 Volts;
  
  int tempVR = 0;
  int currentTempVR = 0;

  // TODO: Tentar o numero mais baixo possivel ao ralenti
  for(int z=0;z<250;z++) { // Corre x para apanhar sempre o topo da onda
    currentTempVR = analogRead(inputVRSensorPin);
    if(currentTempVR > tempVR) tempVR = currentTempVR;
  }

  return tempVR;
}

void stopDutyCycle() {
  dutyCycle = minDutyCycle;
  analogWrite(solenoidPin, dutyCycle);
  return true;
}
