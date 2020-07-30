int   inputAnalogVRSensorPin         = A2;     // set input pin for the VR sensor

int   tempVR                         = 0;
int   currentTempVR                  = 0;
int   inputAnalogVRSensorValue       = 0;      // VR sensor input variable
int   RPMSensorValue                 = 0;
bool  RPMClimbing                    = false;
int   previousRPMSensorValue         = 0;

int   RPMReferenceVoltages[13]       = { };   // RPM Reference table
int   RPMReferenceRevs[13]           = { };   // RPM Reference table

void setup() {
  // Fill RPMReferenceTables - TODO: Put right values
  RPMReferenceRevs[0]  = 0;       RPMReferenceVoltages[0]  = 301;
  RPMReferenceRevs[1]  = 600;     RPMReferenceVoltages[1]  = 410;
  RPMReferenceRevs[2]  = 700;     RPMReferenceVoltages[2]  = 470;
  RPMReferenceRevs[3]  = 800;     RPMReferenceVoltages[3]  = 530;
  RPMReferenceRevs[4]  = 900;     RPMReferenceVoltages[4]  = 580;
  RPMReferenceRevs[5]  = 1000;    RPMReferenceVoltages[5]  = 620;
  RPMReferenceRevs[6]  = 1100;    RPMReferenceVoltages[6]  = 670;
  
  RPMReferenceRevs[7]  = 1200;    RPMReferenceVoltages[7]  = 650;
  RPMReferenceRevs[8]  = 1300;    RPMReferenceVoltages[8]  = 685;
  RPMReferenceRevs[9]  = 1400;    RPMReferenceVoltages[9]  = 725;
  RPMReferenceRevs[10] = 1500;    RPMReferenceVoltages[10] = 765;
  RPMReferenceRevs[11] = 2000;    RPMReferenceVoltages[11] = 920;
  RPMReferenceRevs[12] = 2400;    RPMReferenceVoltages[12] = 1023;
  
  Serial.begin(9600);
}

void loop() {
  // fetch RPM
  // TODO: Tentar o numero mais baixo possivel ao ralenti
  tempVR = 0; // Reset the variable
  for(int z=0;z<1000;z++) { // Corre x para apanhar sempre o topo da onda
    currentTempVR = analogRead(inputAnalogVRSensorPin);
    if(currentTempVR > tempVR) tempVR = currentTempVR;
  }
  inputAnalogVRSensorValue = tempVR;
  // TODO: Fazer a função para converter a voltagem para RPM
  RPMSensorValue = ConvertRPMFromVoltage(inputAnalogVRSensorValue);
  
  if(previousRPMSensorValue > RPMSensorValue) RPMClimbing = false;
  else if (previousRPMSensorValue < RPMSensorValue) RPMClimbing = true;
  else RPMClimbing = false;
    
  previousRPMSensorValue = RPMSensorValue;
  
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
    
  Serial.print("RPM(Volts):       ");
  Serial.println(inputAnalogVRSensorValue);
  Serial.print("RPM:              ");
  Serial.println(RPMSensorValue);
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
