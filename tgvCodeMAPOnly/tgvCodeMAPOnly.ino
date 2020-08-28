// INPUTS
int   inputMaxBoostPotPin             = A0;                   // set input pin for the potentiometer
int   inputHiPowerPotPin              = A1;                   // set input pin for the potentiometer
int   inputAnalogMAPSensorPin         = A3;                   // set input pin for the MAP sensor
int   solenoidPin                     = 3;                    // set output pin for the solenoid

int   inputHiPowerValue               = 0;

int   dutyCycle                       = 45;                   // current PWM value
int   dutyCycleSteps                  = 5;                    // Instead of increasing 1 by 1

// TODO: Must be tested to get these values 
int   minDutyCycle                    = 45;                   // start of end of turbo actuator movement
int   minMaxDutyCycle                 = 60;                   // start of end of turbo actuator movement
int   maxMaxDutyCycle                 = 75;                   // end end of turbo actuator movement
int   maxDutyCycle                    = minMaxDutyCycle;      // start value of turbo actuator movement

int   inputMaxBoostValue              = 0;                    // potentiometer input variable
float MaxBoostValue                   = 0.50;                 // potentiometer value store
float MaxBoostValueLimit              = 1.70;                 // maximum boost allowed above pot definition (1.50 bar)

float startupMAPReading               = 0.00;                 // MAP reading at startup to store zero bar value
float MAPSensorValue                  = 0.00;                 // MAP sensor reading result in bar

void setup() {
  pinMode(inputMaxBoostPotPin, INPUT);
  pinMode(inputHiPowerPotPin, INPUT);
  pinMode(inputAnalogMAPSensorPin, INPUT);
  pinMode(solenoidPin, OUTPUT);                               // declare the solenoidPin as an OUTPUT:
  
  startupMAPReading = analogRead(inputAnalogMAPSensorPin);
  
  Serial.begin(9600);
}

void loop() {
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);                   // Gets maxBoostValue from pot
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;                  // Translates 0-1023 range to 0.5-1.5 Bar
  // TODO: Fazer aqui o inputHiPowerValue para desativar se os 2 pots forem 0
  maxDutyCycle = setMaxDutyCycleFromPot();                                // Set DutyCycle range from pot
  MAPSensorValue = fetchMAP();                                            // fetch MAP

  if(MAPSensorValue > MaxBoostValueLimit || MAPSensorValue > MaxBoostValue) {
    // Lower dutyCycle until below maximum defined level and potentiometer level
    do {
      dutyCycleSteps = 1;
      dutyCycle = dutyCycle - dutyCycleSteps;
      MAPSensorValue = fetchMAP();
    } while (MAPSensorValue > MaxBoostValue);
  } else {
    if(MAPSensorValue < MaxBoostValueLimit && MAPSensorValue <= MaxBoostValue) {
      // Returns 1 to 5 (boost related), depending on map to max diference
      dutyCycleSteps = (int) (((MaxBoostValue - MAPSensorValue) * 10) / 3);
      dutyCycle = dutyCycle + dutyCycleSteps;
    }
  }
  
  if(dutyCycle<minDutyCycle) dutyCycle = minDutyCycle;
  if(dutyCycle>maxDutyCycle) dutyCycle = maxDutyCycle;
  
  //if(inputMaxBoostValue == 0 && inputHiPowerValue == 0) dutyCycle = 0;    // Disable if both pots are 0 !!!
  
  if(dutyCycle>=minDutyCycle && dutyCycle<=maxMaxDutyCycle) 
    analogWrite(solenoidPin, dutyCycle);
  
  // TODO: Ver como fazer isto depois de testar
  //if(inputHiPowerValue == 1023) 
  debug();
}

int setMaxDutyCycleFromPot() {
  inputHiPowerValue = analogRead(inputHiPowerPotPin);
  
  int inputLowBoostValueSteps = 1023 / ( maxMaxDutyCycle - minMaxDutyCycle );
  int stepsAmountInPot = ( inputHiPowerValue / inputLowBoostValueSteps );
  
  if(inputHiPowerValue < inputLowBoostValueSteps) return minMaxDutyCycle;
  else return ( minMaxDutyCycle + stepsAmountInPot );
}

float fetchMAP() {
  int inputAnalogMAPSensorValue = 0;
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  int tempVoltage = (inputAnalogMAPSensorValue - startupMAPReading);
  float tempBoost = (tempVoltage / 400.00);
  return tempBoost;
}

// TODO: Debugging, will be optional from a switch, for now will only be on if first pot is in MAX value
void debug() {
  Serial.println(" ---------------------------------- ");
  Serial.print("SelectedMaxBoost:     ");
  Serial.println(MaxBoostValue);
  Serial.print("MAP(Bar):             ");
  Serial.println(MAPSensorValue);
  Serial.print("SelectedMaxDutyCycle: ");
  Serial.println(inputHiPowerValue);
  Serial.print("PWM MaxDutyCycle:     ");
  Serial.println(maxDutyCycle);
  Serial.print("PWM DutyCycle Steps:  ");
  Serial.println(dutyCycleSteps);
  Serial.print("PWM Duty Cycle:       ");
  Serial.println(dutyCycle);
  Serial.println(" ---------------------------------- ");
}
