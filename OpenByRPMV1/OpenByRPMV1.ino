//RPMReferenceTables
//620 = 1000
//920 = 2000
//1023= 2400

int   inputMaxBoostPotPin            = A0;   // set input pin for the potentiometer
int   inputAnalogVRSensorPin         = A2;   // set input pin for the VR sensor
int   inputAnalogMAPSensorPin        = A3;   // set input pin for the MAP sensor

int   solenoidPin                    = 3;    // set output pin for the solenoid

int   dutyCycleSteps                 = 1;    // Instead of increasing 1 by 1
int   minDutyCycle                   = 210;  // Antes era 115 
int   maxDutyCycle                   = 255;
int   dutyCycle                      = minDutyCycle;

int   inputMaxBoostValue             = 0;    // potentiometer input variable
float MaxBoostValue                  = 0.50; // potentiometer value store
float MaxBoostValueLimit             = 1.59; // maximum value allowed (will be 1.70)

float startupMAPReading              = 0.00; // MAP reading at startup to store zero bar value
float MAPSensorValue                 = 0.00;

int   tempVR                         = 0;
int   currentTempVR                  = 0;
int   inputAnalogVRSensorValue       = 0;    // VR sensor input variable
int   noClimbCounter                 = 0;    // Var p só correr a X necessárias, se n subir + de 30x sai do ciclo

int   consecutiveBelow1000RPM        = 0;

void setup() {
  pinMode(solenoidPin, OUTPUT);               // declare the solenoidPin as an OUTPUT:
  startupMAPReading = analogRead(inputAnalogMAPSensorPin);
  Serial.begin(9600);
}

void loop() {
  Serial.println(" ");
  Serial.println(" ---------------------------------- ");
  
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);  //Gets maxBoostValue from pot
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;
  
  MAPSensorValue = fetchMAP();  // fetch MAP
  
  inputAnalogVRSensorValue = fetchRPM();
  
  //BOOST CONTROL
  if(inputAnalogVRSensorValue >= 650) { // Testar: adicionar || consecutiveBelow1000RPM < 2 
    consecutiveBelow1000RPM = 0;
    if(MAPSensorValue > MaxBoostValueLimit) {
      dutyCycleSteps = 2;
      lowerToMaintainBoost();                           // Lower dutyCycle until below maximum defined level
      Serial.println("lowerToMaintainBoost()"); //APAGAR DPS
    } else {
      if(MAPSensorValue > MaxBoostValue) {
        setDutyCycle('d');                           // Lower dutyCycle until below potentiometer level
        Serial.println("lowerDutyCycle()"); //APAGAR DPS
      } else {
        setDutyCycle('u');
        Serial.println("increaseDutyCycle()"); //APAGAR DPS
      }
    }
  } else {
    consecutiveBelow1000RPM++;
    if(consecutiveBelow1000RPM >= 2) {
      consecutiveBelow1000RPM = 4;
      //if(dutyCycle > minDutyCycle) { //Add later ???
      setDutyCycle('s');
      Serial.println("Below 1000 RPM (620) - After stopping dutyCycle"); //APAGAR DPS
    }
  }
  
  Serial.print("consecutiveBelow1000RPM:");
  Serial.println(consecutiveBelow1000RPM);
  Serial.print("SelectedMaxBoost:       ");
  Serial.println(MaxBoostValue);
  Serial.print("MAP(Bar):               ");
  Serial.println(MAPSensorValue);
  Serial.print("RPM(Volts):             ");
  Serial.println(inputAnalogVRSensorValue);
  Serial.print("PWM DutyCycle Steps:    ");
  Serial.println(dutyCycleSteps);
  Serial.print("PWM Duty Cycle:         ");
  Serial.println(dutyCycle);
  Serial.println(" ---------------------------------- ");
  Serial.println(" ");
}

void lowerToMaintainBoost() {
  do {
    setDutyCycle('d');
    MAPSensorValue = fetchMAP();
  } while (MAPSensorValue > MaxBoostValue);
}

void setDutyCycle(char action) {
  checkDutyCycleSteps();
  switch (action) {
    case 's':
      dutyCycle = minDutyCycle;
      break;
    case 'u':
      checkDutyCycleSteps();
      dutyCycle = dutyCycle + dutyCycleSteps;
      if(dutyCycle > maxDutyCycle) dutyCycle = maxDutyCycle;
      break;
    case 'd':
      checkDutyCycleSteps();
      dutyCycle = dutyCycle - dutyCycleSteps;
      if(dutyCycle < minDutyCycle) dutyCycle = minDutyCycle;
      break;
    default:
      dutyCycle = minDutyCycle;
      break;
  }
  analogWrite(solenoidPin, dutyCycle);
  return true;
}

void checkDutyCycleSteps() {
  if(MAPSensorValue < MaxBoostValue && inputAnalogVRSensorValue > 650) {
    // 1 to 15(or above, boost related), depending on map to max diference
    dutyCycleSteps = (int) ((MaxBoostValue - MAPSensorValue) * 10);
  } else {
    // ver como fazer se houver grandes picos
    dutyCycleSteps = 1;
  }
}

float fetchRPM() {
  int tempVR = 0;
  for(int z=0;z<500;z++) { //Trabalhava bem com 500
    currentTempVR = analogRead(inputAnalogVRSensorPin);
    if(currentTempVR > tempVR) {
      tempVR = currentTempVR;
      noClimbCounter = 0;
    } else {
      noClimbCounter++;
      if(noClimbCounter>=150) break; //Trabalhava bem com 100
    }
  }
  return tempVR;
}

float fetchMAP() {
  int inputAnalogMAPSensorValue = 0;
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  return findBoostFromVoltage(inputAnalogMAPSensorValue);
}

float findBoostFromVoltage(int voltage) {
  int tempVoltage = (voltage - startupMAPReading);
  float boost = (tempVoltage / 400.00);
  return boost;
}
