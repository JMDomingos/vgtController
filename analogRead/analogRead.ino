int   inputMaxBoostPotPin            = A0;     // set input pin for the potentiometer
int   inputLowBoostPotPin            = A1;     // set input pin for the potentiometer
int   inputAnalogVRSensorPin         = A2;     // set input pin for the VR sensor
int   inputAnalogMAPSensorPin        = A3;     // set input pin for the MAP sensor

int   solenoidPin                    = 3;      // set output pin for the solenoid
int   dutyCycle                      = 0;
int   dutyCycleSteps                 = 10;     // Instead of increasing 1 by 1
int   minDutyCycle                   = 45;     // 
int   maxDutyCycle                   = 210;    //

int   inputMaxBoostValue             = 0;      // potentiometer input variable
float MaxBoostValue                  = 0.50;
float MaxBoostValueLimit             = 0.96;
float tempCalcMaxBoostValue          = 0;

int   inputLowBoostValue             = 0;      // potentiometer input variable
int   selectedBoostMap               = 1;

int   inputAnalogVRSensorValue       = 0;      // VR sensor input variable
int   RPMSensorValue                 = 0;

int   inputAnalogMAPSensorValue      = 0;      // MAP sensor input variable
float MAPSensorValue                 = 0.97;
float temp1CalcAnalogMAPSensorValue  = 0;
float temp2CalcAnalogMAPSensorValue  = 0;

float boostValFromArray = 0.00;

int   boostMap1Revs[12]              = { };   // Boost map 1 revs
float boostMap1Pressures[12]         = { };   // Boost map 1 pressures
int   boostMap2Revs[12]              = { };   // Boost map 2 revs
float boostMap2Pressures[12]         = { };   // Boost map 2 pressures
int   boostMap3Revs[12]              = { };   // Boost map 3 revs
float boostMap3Pressures[12]         = { };   // Boost map 3 pressures

int   MAPReferenceVoltages[24]       = { };   // MAP Reference table
float MAPReferenceBoost[24]          = { };   // MAP Reference table

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

  // Fill MAPReferenceTables
  /* //If Vacuum is needed in the future
  -1.00  225
  -0.90  255
  -0.80  285
  -0.70  315
  -0.60  345
  -0.50  380
  -0.40  415
  -0.30  437
  -0.20  480
  -0.10  500
  */
  
  // Fill MAPReferenceTables
  MAPReferenceVoltages[0]  = 540;    MAPReferenceBoost[0]  = 0.00;
  MAPReferenceVoltages[1]  = 560;    MAPReferenceBoost[1]  = 0.05; //check 560
  MAPReferenceVoltages[2]  = 580;    MAPReferenceBoost[2]  = 0.10;
  MAPReferenceVoltages[3]  = 600;    MAPReferenceBoost[3]  = 0.15; //check 600
  MAPReferenceVoltages[4]  = 625;    MAPReferenceBoost[4]  = 0.20; //check 620
  MAPReferenceVoltages[5]  = 660;    MAPReferenceBoost[5]  = 0.25;
  MAPReferenceVoltages[6]  = 670;    MAPReferenceBoost[6]  = 0.30;
  MAPReferenceVoltages[7]  = 685;    MAPReferenceBoost[7]  = 0.35; //check 685
  MAPReferenceVoltages[8]  = 700;    MAPReferenceBoost[8]  = 0.40;
  MAPReferenceVoltages[9]  = 725;    MAPReferenceBoost[9]  = 0.45; //check 725
  MAPReferenceVoltages[10] = 750;    MAPReferenceBoost[10] = 0.50;
  MAPReferenceVoltages[11] = 770;    MAPReferenceBoost[11] = 0.55;
  MAPReferenceVoltages[12] = 800;    MAPReferenceBoost[12] = 0.60;
  MAPReferenceVoltages[13] = 820;    MAPReferenceBoost[13] = 0.65; //check 820
  MAPReferenceVoltages[14] = 840;    MAPReferenceBoost[14] = 0.70;
  MAPReferenceVoltages[15] = 855;    MAPReferenceBoost[15] = 0.75; //check 855
  MAPReferenceVoltages[16] = 870;    MAPReferenceBoost[16] = 0.80;
  MAPReferenceVoltages[17] = 890;    MAPReferenceBoost[17] = 0.85; //check 890
  MAPReferenceVoltages[18] = 910;    MAPReferenceBoost[18] = 0.90;
  MAPReferenceVoltages[19] = 928;    MAPReferenceBoost[19] = 0.95;  //check 928
  MAPReferenceVoltages[20] = 945;    MAPReferenceBoost[20] = 1.00;
  MAPReferenceVoltages[21] = 965;    MAPReferenceBoost[21] = 1.05; //check 965
  MAPReferenceVoltages[22] = 985;    MAPReferenceBoost[22] = 1.10;
//MAPReferenceVoltages[23] = 1000;   MAPReferenceBoost[23] = 1.125;
  MAPReferenceVoltages[23] = 1005;   MAPReferenceBoost[23] = 1.15;  // MAX for this sensor
  /*
  MAPReferenceVoltages[25] = N/A;    MAPReferenceBoost[25] = 1.20;
  MAPReferenceVoltages[26] = N/A;    MAPReferenceBoost[26] = 1.25;
  MAPReferenceVoltages[27] = N/A;    MAPReferenceBoost[27] = 1.30;
  MAPReferenceVoltages[28] = N/A;    MAPReferenceBoost[28] = 1.35;
  MAPReferenceVoltages[29] = N/A;    MAPReferenceBoost[29] = 1.40;
  MAPReferenceVoltages[30] = N/A;    MAPReferenceBoost[30] = 1.45;
  MAPReferenceVoltages[31] = N/A;    MAPReferenceBoost[31] = 1.50;
  */
  
  Serial.begin(9600);
}

void loop() {
  // declare the solenoidPin as an OUTPUT:
  pinMode(solenoidPin, OUTPUT);
  
  //Gets maxBoostValue from pot
  inputMaxBoostValue = analogRead(inputMaxBoostPotPin);
  tempCalcMaxBoostValue = inputMaxBoostValue / 1023.00;
  MaxBoostValue = tempCalcMaxBoostValue + 0.5;
  
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
  
  for(int y=0;y<30;y++) {   // Correr aqui dentro para evitar fazer td constantemente
    // fetch RPM
    inputAnalogVRSensorValue = analogRead(inputAnalogVRSensorPin);
    //RPMSensorValue = inputAnalogVRSensorValue;
    RPMSensorValue = 743; // TODO: For testing purposes
    
    // fetch MAP
    inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);

    Serial.println(" ");
    Serial.println(inputAnalogMAPSensorValue);
    Serial.println(" ");
    
    MAPSensorValue = findBoostFromVoltage(inputAnalogMAPSensorValue);
    
    if(RPMSensorValue <= 1000 && MAPSensorValue > 0.05 ) {
      //Lower the PWM Duty Cycle to 0%
      dutyCycle = 0;
      // Apply dutyCycle
      applyDutyCycle(dutyCycle);
      //continue; // Só continua se passar o dutyCycle à porta aqui dentro
    }

    
    
    // FIXME: Porque é que não consigo ver o valor do array ?????
    //boostValFromArray = boostMaps[selectedBoostMap][1400];
    //Serial.println( boostValFromArray );
    
    //findPreviousRPMKey()
    //findNextRPMKey()

    Serial.println(" ");
    Serial.println(" ");

    // Testar o pointer do array
    
    Serial.print("Boost Map:        ");
    Serial.println(selectedBoostMap);
    Serial.print("SelectedMaxBoost: ");
    Serial.println(MaxBoostValue);
    Serial.print("MaxBoost:         ");
    Serial.println(MaxBoostValueLimit);
    Serial.print("RPM(Volts):       ");
    Serial.println(inputAnalogVRSensorValue);
    Serial.print("RPM:              ");
    Serial.println(RPMSensorValue);
    Serial.print("MAP(Bar):         ");
    Serial.println(findBoostFromVoltage(inputAnalogMAPSensorValue));
    Serial.print("PWM Duty Cycle:   ");
    Serial.println(dutyCycle);
    
    delay(500); // Mudar para 100 depois???
  }
}

float findBoostFromVoltage(int voltage) {
  float boost = 1.50;

  if(voltage > 1005) {
    Serial.println("Error: Overboost !!!");
    return MaxBoostValueLimit;
  }
  
  for(int x=0;x<sizeof(MAPReferenceVoltages);x++) {
    if(MAPReferenceVoltages[x] <= voltage) {
      boost = MAPReferenceBoost[x];
    } else if (MAPReferenceVoltages[x] > voltage) {
      return  boost;
    } else {
      Serial.println("Error: Boost off range !!!");
      return MaxBoostValueLimit; // Is this a good idea???
    }
  }
  
  // Se falhar algo devolve o máximo
  return MaxBoostValueLimit;
}

void increaseDutyCycle() {
  if(applyDutyCycle(dutyCycle + dutyCycleSteps) != true) {
    Serial.println("Error: increaseDutyCycle() - Bad call!");
  }
}

void lowerDutyCycle() {
  if(applyDutyCycle(dutyCycle - dutyCycleSteps) != true) {
    Serial.println("Error: lowerDutyCycle() - Bad call!");
  }
}

bool applyDutyCycle(int dutyCycleValue) {
  if(dutyCycleValue <0 && dutyCycleValue > 255) {
    Serial.println("Error: PWM Duty Cycle off range !!!");
    return false;
  }

  if(dutyCycleValue != 0 && dutyCycleValue < minDutyCycle && dutyCycleValue > maxDutyCycle) {
    Serial.println("Error: PWM Duty Cycle off configured range !!!");
    return false;
  }

  // Outros controlos
  
  // send the square wave signal to the solenoid:
  analogWrite(solenoidPin, dutyCycleValue);
  return true;
}

/*
int findRPMKeys() {
  int lastFoundRPM = 0;
  int currentRPM = 0;
  for(int x=1000;x<=3900;x=x+50) {
    currentRPM = x;
    if(boostMaps[selectedBoostMap][x]) {
      lastFoundRPM = boostMaps[selectedBoostMap][x];
    } else {
      if(x > RPMSensorValue) return lastFoundRPM; //Tem de mudar para a var certa qd tiver RPM
    }
  }

  return lastFoundRPM;
}
*/
