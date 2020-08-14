// INPUTS
int   inputMaxBoostPotPin                   = A0;   // set input pin for the potentiometer
int   inputLowBoostPotPin                   = A1;     // set input pin for the potentiometer
int   inputDigitalVRSensorPin               = 4;    // set input pin for the VR sensor
int   inputAnalogMAPSensorPin               = A3;   // set input pin for the MAP sensor

// PWM Solenoid Output
int   solenoidPin                           = 3;    // set output pin for the solenoid

int   dutyCycle                             = 245;  // current PWM value
int   dutyCycleSteps                        = 1;    // Instead of increasing 1 by 1
int   minDutyCycle                          = 100;  // Must be tested to get these values
int   maxDutyCycle                          = 255;  // start and end of turbo actuator movement

int   inputLowBoostValue                    = 0;    // potentiometer input variable
int   selectedBoostMap                      = 1;

int   inputMaxBoostValue                    = 0;    // potentiometer input variable
float MaxBoostValue                         = 0.50; // potentiometer value store
float MaxBoostValueLimit                    = 1.70; // maximum boost allowed above pot definition (1.50 bar)

float boostTolerance                        = 0.05; // Tolerance to ignore action inside loop
float startupMAPReading                     = 0.00; // MAP reading at startup to store zero bar value
float MAPSensorValue                        = 0.00; // MAP sensor reading result in bar

int   inputAnalogVRSensorValue              = 0;    // VR sensor input variable
int   RPMSensorValue                        = 0;
int   RPMSensorValueCorrectionFactor        = 1.25;

unsigned long duration                      = 0;    // Microsecs LOW on camshaft


int   previousRPMSensorValue                = 0;
bool  RPMClimbing                           = false;

// TODO: Any remaining variables

float boostValFromArray                     = 0.00;

int   boostMap1Revs[12]                     = { };  // Boost map 1 revs
float boostMap1Pressures[12]                = { };  // Boost map 1 pressures
int   boostMap2Revs[12]                     = { };  // Boost map 2 revs
float boostMap2Pressures[12]                = { };  // Boost map 2 pressures
int   boostMap3Revs[12]                     = { };  // Boost map 3 revs
float boostMap3Pressures[12]                = { };  // Boost map 3 pressures

int   maxDutyCyclePerRPMs[5]                = { };  // RPM Reference table
int   maxDutyCyclePerRPM[5]                 = { };  // RPM Reference table


void setup() {
  // Fill boostMap1
  boostMap1Revs[0]  = 1400;       boostMap1Pressures[0]  = 0.10;
  boostMap1Revs[1]  = 1600;       boostMap1Pressures[1]  = 0.20; 
  boostMap1Revs[2]  = 1800;       boostMap1Pressures[2]  = 0.30;
  boostMap1Revs[3]  = 2000;       boostMap1Pressures[3]  = 0.40; 
  boostMap1Revs[4]  = 2250;       boostMap1Pressures[4]  = 0.55;
  boostMap1Revs[5]  = 2500;       boostMap1Pressures[5]  = 0.75; 
  boostMap1Revs[6]  = 2750;       boostMap1Pressures[6]  = 0.95; 
  boostMap1Revs[7]  = 3000;       boostMap1Pressures[7]  = 1.15;
  boostMap1Revs[8]  = 3250;       boostMap1Pressures[8]  = 1.25;
  boostMap1Revs[9]  = 3500;       boostMap1Pressures[9]  = 1.35; 
  boostMap1Revs[10] = 3750;       boostMap1Pressures[10] = 1.40; 
  boostMap1Revs[11] = 3900;       boostMap1Pressures[11] = 1.50;

  // Fill boostMap2
  boostMap2Revs[0]  = 1250;       boostMap2Pressures[0]  = 0.10;
  boostMap2Revs[1]  = 1300;       boostMap2Pressures[1] = 0.20;
  boostMap2Revs[2]  = 1400;       boostMap2Pressures[2] = 0.35; 
  boostMap2Revs[3]  = 1500;       boostMap2Pressures[3] = 0.45; 
  boostMap2Revs[4]  = 1650;       boostMap2Pressures[4] = 0.50; 
  boostMap2Revs[5]  = 1850;       boostMap2Pressures[5] = 0.60; 
  boostMap2Revs[6]  = 2000;       boostMap2Pressures[6] = 0.75; 
  boostMap2Revs[7]  = 2250;       boostMap2Pressures[7] = 0.90; 
  boostMap2Revs[8]  = 2500;       boostMap2Pressures[8] = 1.05; 
  boostMap2Revs[9]  = 2750;       boostMap2Pressures[9] = 1.25; 
  boostMap2Revs[10] = 3000;       boostMap2Pressures[10] = 1.35;
  boostMap2Revs[11] = 3250;       boostMap2Pressures[11] = 1.50;

  // Fill boostMap3
  boostMap3Revs[0]  = 1000;       boostMap3Pressures[0]  = 0.10; 
  boostMap3Revs[1]  = 1100;       boostMap3Pressures[1]  = 0.15; 
  boostMap3Revs[2]  = 1200;       boostMap3Pressures[2]  = 0.20; 
  boostMap3Revs[3]  = 1300;       boostMap3Pressures[3]  = 0.30; 
  boostMap3Revs[4]  = 1400;       boostMap3Pressures[4]  = 0.50; 
  boostMap3Revs[5]  = 1500;       boostMap3Pressures[5]  = 0.65;
  boostMap3Revs[6]  = 1600;       boostMap3Pressures[6]  = 0.70; 
  boostMap3Revs[7]  = 1800;       boostMap3Pressures[7]  = 0.90; 
  boostMap3Revs[8]  = 2000;       boostMap3Pressures[8]  = 1.00; 
  boostMap3Revs[9]  = 2200;       boostMap3Pressures[9]  = 1.25; 
  boostMap3Revs[10] = 2300;       boostMap3Pressures[10] = 1.35; 
  boostMap3Revs[11] = 2500;       boostMap3Pressures[11] = 1.50;
  
  // Fill maxDutyCycle p/ RPM Array
  // TODO: Make maps for right pot
  // TODO: Correct values with testing
  maxDutyCyclePerRPMs[0] = 1000;      maxDutyCyclePerRPM[0] = maxDutyCycle;
  maxDutyCyclePerRPMs[1] = 1200;      maxDutyCyclePerRPM[1] = 250;
  maxDutyCyclePerRPMs[2] = 1500;      maxDutyCyclePerRPM[2] = 245;
  maxDutyCyclePerRPMs[3] = 2000;      maxDutyCyclePerRPM[3] = 240;
  maxDutyCyclePerRPMs[4] = 2400;      maxDutyCyclePerRPM[4] = 235;
  
  
  pinMode(inputDigitalVRSensorPin, INPUT);
  pinMode(solenoidPin, OUTPUT);               // declare the solenoidPin as an OUTPUT:
  
  startupMAPReading = analogRead(inputAnalogMAPSensorPin);
  
  Serial.begin(9600);
}

void loop() {
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);  //Gets maxBoostValue from pot
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;

  //Gets boostMap from pot
  inputLowBoostValue = analogRead(inputLowBoostPotPin);
  if (inputLowBoostValue < 341) {
    selectedBoostMap = 1;
    
    int   *boostMapRevs       = boostMap1Revs;
    float *boostMap1Pressures = boostMap1Pressures;
  } else if (inputLowBoostValue < 682) {
    selectedBoostMap = 2;

    int   *boostMapRevs       = boostMap2Revs;
    float *boostMap1Pressures = boostMap2Pressures;
  } else {
    selectedBoostMap = 3;
    
    int   *boostMapRevs       = boostMap3Revs;
    float *boostMap1Pressures = boostMap3Pressures;
  }

  // Correr aqui dentro para evitar fazer td constantemente
  for(int y=0;y<5000;y++) {
    // fetch RPM
    RPMSensorValue = fetchRPM();
    
    // Redefine maxDutyCycle for current RPM
    maxDutyCycle = convertMaxDutyCycleFromRPM(RPMSensorValue);
    
    if(previousRPMSensorValue > RPMSensorValue) RPMClimbing = false;
    else if (previousRPMSensorValue < RPMSensorValue) RPMClimbing = true;
    else RPMClimbing = false;
    
    MAPSensorValue = fetchMAP();  // fetch MAP
    
    
    // STARTS HERE
    
    
    
    
    
    /*
    if(RPMSensorValue < 1000) { 
      if(dutyCycle > minDutyCycle) {
        stopDutyCycle();
        debug("Below 1000 - After stopping dutyCycle"); //APAGAR DPS
        continue;
      }
    } else if(RPMSensorValue >= 1000 && RPMSensorValue < 2400) {
      if(MAPSensorValue > MaxBoostValueLimit) {
        lowerToMaintainBoost();                           // Lower dutyCycle until below maximum defined level
        continue;
      }
      if(MAPSensorValue > MaxBoostValue) {
        lowerToMaintainBoost();                           // Lower dutyCycle until below potentiometer level
        continue;
      }
      if(MAPSensorValue < MaxBoostValueLimit) {
        if(RPMClimbing && MAPSensorValue >= (MaxBoostValue - boostTolerance)) {
          debug("Between 1000 and 2400 RPMClimbing MAP Almost Max - does nothing"); //APAGAR DPS
          continue; // Skip if close to make boost
        } else if(RPMClimbing && MAPSensorValue <= (MaxBoostValue - boostTolerance)) {
          debug("Between 1000 and 2400 RPMClimbing MAP Under (Max - Tolerance) - Before increasing dutyCycle"); //APAGAR DPS
          increaseDutyCycle();
          //debug("Between 1000 and 2400 RPMClimbing MAP Under (Max - Tolerance) - After increasing dutyCycle"); //APAGAR DPS
          continue;
        } else {
          if(!RPMClimbing) {
            //RPM going down depends on rate of descent
            if((previousRPMSensorValue - RPMSensorValue) < RPMChangeUnderLoadPerCycleTolerance)
              // Engine losing RPM under load
              debug("Between 1000 and 2400 !RPMClimbing MAP Under Tolerance - does nothing"); //APAGAR DPS
              continue;
            if((previousRPMSensorValue - RPMSensorValue) > RPMChangeUnderLoadPerCycleTolerance) {
              // Let go of the acelerator 
              debug("Between 1000 and 2400 !RPMClimbing Going down fast - does nothing"); //APAGAR DPS
              // Lower with a while as revs go down in a function ???
            }
          }
        }
      }
      while(dutyCycle < maxDutyCycle && MAPSensorValue < MaxBoostValue) {
        debug("Between 1000 and 2400 - Before Increasing DutyCycle"); //APAGAR DPS
        increaseDutyCycle();
        //debug("Between 1000 and 2400 - After Increasing DutyCycle"); //APAGAR DPS
      }
    } else {                                              // Above 2400 RPM
      if(MAPSensorValue >= MaxBoostValueLimit) {
        lowerToMaintainBoost();                           // Lower dutyCycle if above maximum defined level
        continue;
      }
      if(MAPSensorValue >= MaxBoostValue) {
        lowerToMaintainBoost();                           // Lower dutyCycle if above potentiometer defined level
        continue;
      }
      if(MAPSensorValue < MaxBoostValue) {
        debug("Above 2400 - Before Increase DutyCycle"); //APAGAR DPS
        increaseDutyCycle();
        //debug("Above 2400 - After Increase DutyCycle"); //APAGAR DPS
        
      } 
    }
    */
    // ENDS HERE
    
    
    previousRPMSensorValue = RPMSensorValue;
  }
  
  debug("End of Cycle");
}



int convertMaxDutyCycleFromRPM(int rpm) {
  if(rpm <= maxDutyCyclePerRPMs[0]) return maxDutyCyclePerRPM[0];
  else if(rpm >= maxDutyCyclePerRPMs[4]) return maxDutyCyclePerRPM[4];
  else {
    int tempDutyCycle = 0;
    for(int x=0;x<sizeof(maxDutyCyclePerRPMs);x++) { 
      if(rpm <= maxDutyCyclePerRPMs[x]) tempDutyCycle = maxDutyCyclePerRPM[x];
      else break;
    }
    return tempDutyCycle;
  }
}


int fetchRPM() {
  int rpmX = 0;
  do {
    rpmX++;
    duration = pulseInLong(inputDigitalVRSensorPin, LOW, 200000);
    if(rpmX >= 5000) {
      // engine is off ... or rotating really fast ??? (check max - duration < 1000)
      // O que fazer com o motor desligado e a chave ligada para não ficar preso neste ciclo?
      // Talvez por o pwm a 0 e fazer um delay dentro do ciclo ...
    }
  } while(duration<1000);
  
  // Multiplicar rpm p/ correctionFactor ???
  rpm = (int) ( 60.00 / ( ( duration / 1000000.00 ) * 3.00 ) ); // converte pulse time para rpm
  return (rpm * RPMSensorValueCorrectionFactor);
}

float fetchMAP() {
  int inputAnalogMAPSensorValue = 0;
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  int tempVoltage = (inputAnalogMAPSensorValue - startupMAPReading);
  float tempBoost = (tempVoltage / 400.00);
  return tempBoost;
}

int boostDifferencial() {
  // Returns 1 to 15, depending on map to max diference
  int boostDifferencial = (int) ((MaxBoostValue - MAPSensorValue) * 10);
  return boostDifferencial;
}

void lowerToMaintainBoost() {
  do {
    setDutyCycle('d');
    MAPSensorValue = fetchMAP();
  } while (MAPSensorValue > MaxBoostValue);
}


void checkDutyCycleSteps() {
  // TODO: RPMSensorValue below is still in voltage
  if(MAPSensorValue < MaxBoostValue && RPMSensorValue > 1000) { 
    dutyCycleSteps = boostDifferencial(); // 1 to 15(or above, boost related), depending on map to max diference
  } else {
    dutyCycleSteps = 15; 
  }
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

void stopDutyCycle() {
  dutyCycle = minDutyCycle;
  analogWrite(solenoidPin, dutyCycle);
  return true;
}

void increaseDutyCycle() {
  checkDutyCycleSteps();
  dutyCycle = dutyCycle + dutyCycleSteps;
  if(dutyCycle > maxDutyCycle) return false;
  else {
    analogWrite(solenoidPin, dutyCycle);
    return true;
  }
}

void lowerDutyCycle() {
  checkDutyCycleSteps();
  dutyCycle = dutyCycle - dutyCycleSteps;
  if(dutyCycle < minDutyCycle) return false;
  else {
    analogWrite(solenoidPin, dutyCycle);
    return true;
  }
}

// TODO: Debugging, will be optional from a switch, for now will only be on if first pot is in MAX value
// TODO: Verify all vars printed here
void debug(String identifier) {
  Serial.println(" ---------------------------------- ");
  Serial.println(identifier);
  Serial.print("SelectedMaxBoost:     ");
  Serial.println(MaxBoostValue);
  Serial.print("MAP(Bar):             ");
  Serial.println(MAPSensorValue);
  Serial.print("RPM:                  ");
  Serial.println(RPMSensorValue);
  Serial.print("PWM MaxDutyCycle:     ");
  Serial.println(maxDutyCycle);
  Serial.print("PWM DutyCycle Steps:  ");
  Serial.println(dutyCycleSteps);
  Serial.print("PWM Duty Cycle:       ");
  Serial.println(dutyCycle);
}
