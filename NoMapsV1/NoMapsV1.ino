int   inputMaxBoostPotPin                   = A0;   // set input pin for the potentiometer
int   inputAnalogVRSensorPin                = A2;   // set input pin for the VR sensor
int   inputAnalogMAPSensorPin               = A3;   // set input pin for the MAP sensor

int   solenoidPin                           = 3;    // set output pin for the solenoid

int   dutyCycle                             = 245;
int   dutyCycleSteps                        = 1;    // Instead of increasing 1 by 1
int   minDutyCycle                          = 115;   
int   maxDutyCycle                          = 245;   

int   inputMaxBoostValue                    = 0;    // potentiometer input variable
float MaxBoostValue                         = 0.50; // potentiometer value store
float MaxBoostValueLimit                    = 0.99; // maximum value allowed (will be 1.70)

float boostTolerance                        = 0.05; // Tolerance to ignore action inside loop
float startupMAPReading                     = 0.00; // MAP reading at startup to store zero bar value
float MAPSensorValue                        = 0.00;

int   inputAnalogVRSensorValue              = 0;    // VR sensor input variable
int   RPMSensorValue                        = 0;
int   previousRPMSensorValue                = 0;
bool  RPMClimbing                           = false;
int   RPMChangeUnderLoadPerCycleTolerance   = 10;   // Needs testing with serial.print to find a good value

int   maxDutyCyclePerRPMVolts[5]            = { };  // RPM Reference table
int   maxDutyCyclePerRPM[5]                 = { };  // RPM Reference table

void setup() {
  // Fill maxDutyCycle p/ RPM Array - TODO: Put right values and make maps for right pot
  maxDutyCyclePerRPMVolts[0] = 580;      maxDutyCyclePerRPM[0] = maxDutyCycle;
  maxDutyCyclePerRPMVolts[1] = 650;      maxDutyCyclePerRPM[1] = 250;  // 1200 RPM
  maxDutyCyclePerRPMVolts[2] = 765;      maxDutyCyclePerRPM[2] = 245;  // 1500 RPM
  maxDutyCyclePerRPMVolts[3] = 920;      maxDutyCyclePerRPM[4] = 240;  // 2000 RPM 
  maxDutyCyclePerRPMVolts[4] = 1020;     maxDutyCyclePerRPM[5] = 235;  // 2400 RPM 
  
  pinMode(solenoidPin, OUTPUT);               // declare the solenoidPin as an OUTPUT:
  
  startupMAPReading = analogRead(inputAnalogMAPSensorPin);
  
  Serial.begin(9600);
}

void loop() {
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);  //Gets maxBoostValue from pot
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;

  for(int y=0;y<100;y++) {   // Correr aqui dentro para evitar fazer td constantemente
    // fetch RPM
    RPMSensorValue = fetchRPM();
    
    // Redefine maxDutyCycle for current RPM
    maxDutyCycle = convertMaxDutyCycleFromRPMVoltage(RPMSensorValue);
    
    if(previousRPMSensorValue > RPMSensorValue) RPMClimbing = false;
    else if (previousRPMSensorValue < RPMSensorValue) RPMClimbing = true;
    else RPMClimbing = false;
    
    MAPSensorValue = fetchMAP();  // fetch MAP
    
    //BOOST CONTROL
    if(RPMSensorValue < 580) {                            // Below 1000 RPM
      if(dutyCycle > minDutyCycle) {
        stopDutyCycle();
        Serial.println("Below 1000 - After stopping dutyCycle"); //APAGAR DPS
        continue;
      }
    } else if(RPMSensorValue >= 580 && RPMSensorValue < 1000) { // 1000 to 2400 RPM
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
    
    
    
    //delay(100);
    
    // Saves here to have in the next loop
    previousRPMSensorValue = RPMSensorValue;
  }
}

float fetchMAP() {
  int inputAnalogMAPSensorValue = 0;
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  return findBoostFromVoltage(inputAnalogMAPSensorValue);
}

void lowerToMaintainBoost() {
  do {
    lowerDutyCycle();                // Lower dutyCycle until below maximum defined level
    //delay(50);
    MAPSensorValue = fetchMAP();
  } while (MAPSensorValue > MaxBoostValueLimit);
}

float findBoostFromVoltage(int voltage) {
  int tempVoltage = (voltage - startupMAPReading);
  float boost = (tempVoltage / 400.00);
  return boost;
}

int convertMaxDutyCycleFromRPMVoltage(int voltage) {
  if(voltage <= maxDutyCyclePerRPMVolts[0]) 
    return maxDutyCyclePerRPM[0];
  else if(voltage >= maxDutyCyclePerRPMVolts[4]) 
    return maxDutyCyclePerRPM[4];
  else {
    int tempDutyCycle = 0;
    for(int x=0;x<sizeof(maxDutyCyclePerRPMVolts);x++) {
      if(voltage <= maxDutyCyclePerRPMVolts[x])
        tempDutyCycle = maxDutyCyclePerRPM[x];
      else
        break;
    }
    return tempDutyCycle;
  }
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
  //for(int z=0;z<250;z++) { // Corre x para apanhar sempre o topo da onda
  //  currentTempVR = analogRead(inputAnalogVRSensorPin);
  //  if(currentTempVR > tempVR) tempVR = currentTempVR;
  //}
  tempVR = analogRead(inputAnalogVRSensorPin);
  return tempVR;
}

int boostDifferencial() {
  // Returns 1 to 15, depending on map to max diference
  int boostDifferencial = (int) ((MaxBoostValue - MAPSensorValue) * 10);
  return boostDifferencial;
}

void checkDutyCycleSteps() {
  if(MAPSensorValue < MaxBoostValue && RPMSensorValue > 580) {
    dutyCycleSteps = boostDifferencial(); // 1 to 15(or above, boost related), depending on map to max diference
  } else {
    dutyCycleSteps = 15; 
  }
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
