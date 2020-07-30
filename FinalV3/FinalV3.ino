int   inputMaxBoostPotPin            = A0;     // set input pin for the potentiometer
int   inputSelectMapPotPin           = A1;     // set input pin for the potentiometer
int   inputAnalogVRSensorPin         = A2;     // set input pin for the VR sensor
int   inputAnalogMAPSensorPin        = A3;     // set input pin for the MAP sensor

int   solenoidPin                    = 3;      // set output pin for the solenoid

int   dutyCycle                      = 15;
int   dutyCycleSteps                 = 5;      // Instead of increasing 1 by 1
int   minDutyCycle                   = 0;     // TODO: Check the value 
int   maxDutyCycle                   = 250;     // TODO: Check the value

int   inputMaxBoostValue             = 0;      // potentiometer input variable
float MaxBoostValue                  = 0.50;   // potentiometer value store
float MaxBoostValueLimit             = 1.51;

int   inputSelectMapValue            = 0;      // potentiometer input variable
int   selectedBoostMap               = 1;

int   tempVR                         = 0;
int   currentTempVR                  = 0;
int   inputAnalogVRSensorValue       = 0;      // VR sensor input variable
int   RPMSensorValue                 = 0;

int   inputAnalogMAPSensorValue      = 0;      // MAP sensor input variable
// FIXME: Ver se por causa de ser float não guarda mal o valor inicial que é int
float startupMAPReading              = 0.00;   // MAP reading at startup to store zero bar value
float MAPSensorValue                 = 0.00;

int   boostMapsSize = 12;  // TODO: test in boost maps declarations

int   boostMapRevs[12]               = { };
float boostMapPressures[12]          = { };

int   boostMap1Revs[12]              = { };   // Boost map 1 revs
float boostMap1Pressures[12]         = { };   // Boost map 1 pressures
int   boostMap2Revs[12]              = { };   // Boost map 2 revs
float boostMap2Pressures[12]         = { };   // Boost map 2 pressures
int   boostMap3Revs[12]              = { };   // Boost map 3 revs
float boostMap3Pressures[12]         = { };   // Boost map 3 pressures

int   RPMReferenceVoltages[13]       = { };   // RPM Reference table
int   RPMReferenceRevs[13]           = { };   // RPM Reference table

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
  boostMap3Revs[0]  = 1000;       boostMap3Pressures[0]  = 0.05; 
  boostMap3Revs[1]  = 1100;       boostMap3Pressures[1]  = 0.10; 
  boostMap3Revs[2]  = 1200;       boostMap3Pressures[2]  = 0.20; 
  boostMap3Revs[3]  = 1300;       boostMap3Pressures[3]  = 0.30; 
  boostMap3Revs[4]  = 1400;       boostMap3Pressures[4]  = 0.40; 
  boostMap3Revs[5]  = 1600;       boostMap3Pressures[5]  = 0.50;
  boostMap3Revs[6]  = 1800;       boostMap3Pressures[6]  = 0.60; 
  boostMap3Revs[7]  = 2000;       boostMap3Pressures[7]  = 0.75; 
  boostMap3Revs[8]  = 2100;       boostMap3Pressures[8]  = 0.90; 
  boostMap3Revs[9]  = 2200;       boostMap3Pressures[9]  = 1.10; 
  boostMap3Revs[10] = 2300;       boostMap3Pressures[10] = 1.30; 
  boostMap3Revs[11] = 2400;       boostMap3Pressures[11] = 1.50;

  // Fill RPMReferenceTables - TODO: Put right values
  RPMReferenceRevs[0] = 0;        RPMReferenceVoltages[0] = 305;
  RPMReferenceRevs[1] = 600;      RPMReferenceVoltages[1] = 410;
  RPMReferenceRevs[2] = 700;      RPMReferenceVoltages[2] = 450;
  RPMReferenceRevs[3] = 800;      RPMReferenceVoltages[3] = 490;
  RPMReferenceRevs[4] = 900;      RPMReferenceVoltages[4] = 510;
  RPMReferenceRevs[5] = 1000;     RPMReferenceVoltages[5] = 580;
  RPMReferenceRevs[6] = 1100;     RPMReferenceVoltages[6] = 615;
  RPMReferenceRevs[7] = 1200;     RPMReferenceVoltages[7] = 650;
  RPMReferenceRevs[8] = 1300;     RPMReferenceVoltages[8] = 685;
  RPMReferenceRevs[9] = 1400;     RPMReferenceVoltages[9] = 725;
  RPMReferenceRevs[10] = 1500;    RPMReferenceVoltages[10] = 765;
  RPMReferenceRevs[11] = 2000;    RPMReferenceVoltages[11] = 920;
  RPMReferenceRevs[12] = 2400;    RPMReferenceVoltages[12] = 1023;
  
  // declare the solenoidPin as an OUTPUT:
  pinMode(solenoidPin, OUTPUT);

  startupMAPReading = analogRead(inputAnalogMAPSensorPin);
  
  Serial.begin(9600);
}

void loop() {
  //Gets maxBoostValue from pot
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);
  MaxBoostValue = (inputMaxBoostValue / 1023.00) + 0.50;
  
  //Gets boostMap from pot
  inputSelectMapValue = analogRead(inputSelectMapPotPin);
  
  if (inputSelectMapValue < 341) {
    selectedBoostMap             = 1;
    for (int o=0; o<boostMapsSize; o++) 
      boostMapRevs[o] = boostMap1Revs[o];
    for (int o=0; o<boostMapsSize; o++) 
      boostMapPressures[o] = boostMap1Pressures[o];
    
  } else if (inputSelectMapValue < 682) {
    selectedBoostMap             = 2;
    for (int o=0; o<boostMapsSize; o++) 
      boostMapRevs[o] = boostMap2Revs[o];
    for (int o=0; o<boostMapsSize; o++) 
      boostMapPressures[o] = boostMap2Pressures[o];
    
  } else {
    selectedBoostMap             = 3;
    for (int o=0; o<boostMapsSize; o++) 
      boostMapRevs[o] = boostMap3Revs[o];
    for (int o=0; o<boostMapsSize; o++) 
      boostMapPressures[o] = boostMap3Pressures[o];
  }
  
  for(int y=0;y<30;y++) {   // Correr aqui dentro para evitar fazer td constantemente
    // fetch RPM
    // TODO: Tentar o numero mais baixo possivel ao ralenti
    tempVR = 0; // Reset the variable
    for(int z=0;z<200;z++) { // Corre x para apanhar sempre o topo da onda
      currentTempVR = analogRead(inputAnalogVRSensorPin);
      if(currentTempVR > tempVR) tempVR = currentTempVR;
    }
    inputAnalogVRSensorValue = tempVR;
    // TODO: Fazer a função para converter a voltagem para RPM
    RPMSensorValue = ConvertRPMFromVoltage(inputAnalogVRSensorValue);
    
    //RPMSensorValue = ConvertRPMFromVoltage(985);                         // DELETE - FOR TESTING PURPOSES ONLY

    inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
    MAPSensorValue = findBoostFromVoltage(inputAnalogMAPSensorValue);

    //BOOST CONTROL
    if(RPMSensorValue < 1000) {
      //stopDutyCycle(); Causes the same problem
      
      if(MAPSensorValue > 0.00) {
        lowerDutyCycle();                                   // Lower dutyCycle on revs drop - test if better with stopDutyCycle();
        continue;                                           // skip the rest after applying
      }
      
    } else {
      // above 1000 rpm regardless of boost
      if(MAPSensorValue < 0.05 && dutyCycle < 255) { // FIXME: Passar para a variável
        increaseDutyCycle();                              // Increase dutyCycle to start boost
        continue;                                         // skip the rest after applying
      }
      
      if(MAPSensorValue > MaxBoostValueLimit) {           // above maximum turbo boost limit
        lowerDutyCycle();                                 // Lower dutyCycle if above maximum defined level
        continue;                                         // skip the rest after applying
      } else if (MAPSensorValue > MaxBoostValue) {
        lowerDutyCycle();                                 // Lower dutyCycle if above maximum pot level
        continue;                                         // skip the rest after applying
      } else { // above 1000 rpm's boost between 0.00 and MaxBoostValueLimit
        
        Serial.println("Entrou no else! ");
        
        /*
        int revsStep = 0;
        for(int a=0;a<boostMapsSize;a++) {
          if(boostMapRevs[a] <= RPMSensorValue) revsStep = a; // Current rev step
          else break;
        }
        
        if(MAPSensorValue < boostMapPressures[revsStep]) {
          increaseDutyCycle();
        } else {
          lowerDutyCycle();
        }
        */
        
      }
    }
    
    Serial.println(" ");
    
    // TODO: Testar o pointer do array
    
    Serial.print("SelectedMaxBoost: ");
    Serial.println(MaxBoostValue);
    //Serial.print("MaxBoostLimit:    ");
    //Serial.println(MaxBoostValueLimit);
    Serial.print("Boost Map:        ");
    Serial.println(selectedBoostMap);
    //Serial.print("RPM(Volts):       ");
    //Serial.println(inputAnalogVRSensorValue);
    Serial.print("RPM:              ");
    Serial.println(RPMSensorValue);
    //Serial.print("MAP(Volts):       ");
    //Serial.println(inputAnalogMAPSensorValue);
    Serial.print("MAP(Bar):         ");
    Serial.println(findBoostFromVoltage(inputAnalogMAPSensorValue));
    Serial.print("PWM Duty Cycle:   ");
    Serial.println(dutyCycle);
    
    delay(50); // é 100, 300 para ver, 500 para ver melhor
  }
}

int ConvertRPMFromVoltage(int voltage) {
  if(voltage > 1020) 
    return 2500; // FIXME: Temporary until RPM fix
  
  int tempVoltage = 0;
  for(int x=0;x<sizeof(RPMReferenceVoltages);x++) {
    if(voltage >= RPMReferenceVoltages[x]) tempVoltage = RPMReferenceVoltages[x];
    else return RPMReferenceRevs[(x-1)];
  }
  
  return false; // TODO: Testar se devolve zero
  
}

float findBoostFromVoltage(int voltage) {
  int tempVoltage = (voltage - startupMAPReading);
  float boost = (tempVoltage / 400.00);
  
  if(boost > MaxBoostValueLimit) {
    lowerDutyCycle();
    Serial.println("Error: Overboost !!!");
  }
  
  return boost;
}

void increaseDutyCycle() {
  dutyCycle = dutyCycle + dutyCycleSteps;
  if(applyDutyCycle(dutyCycle) != true) {
    Serial.println("Error: increaseDutyCycle() - Bad call!");
  }
}

void lowerDutyCycle() {
  dutyCycle = dutyCycle - dutyCycleSteps;
  if(applyDutyCycle(dutyCycle) != true) {
    Serial.println("Error: lowerDutyCycle() - Bad call!");
  }
}

void stopDutyCycle() {
  dutyCycle = minDutyCycle;
  if(applyDutyCycle(minDutyCycle) != true) {
    Serial.println("Error: stopDutyCycle() - Bad call!");
  }
}

bool applyDutyCycle(int dutyCycleValue) {
  
  if(dutyCycleValue <0 && dutyCycleValue > 255) {
    Serial.println("Error: PWM Duty Cycle off range !!!");
    return false;
  }

  // FIXME: Requires test to discover values before field test
  if(dutyCycleValue > 0 && dutyCycleValue < 255 && dutyCycleValue < minDutyCycle && dutyCycleValue > maxDutyCycle) {
    Serial.println("Error: PWM Duty Cycle off configured range !!!");
    return false;
  }
  
  // TODO: Outros controlos ???

  //dutyCycle = dutyCycleValue;
  // send the square wave signal to the solenoid:
  analogWrite(solenoidPin, dutyCycleValue);
  return true;
}
