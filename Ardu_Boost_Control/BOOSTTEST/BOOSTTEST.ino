const int   BoostSelectPin                   = A0;   // set input pin for the potentiometer
const int   VRpin                = A2;   // set input pin for the VR sensor
const int   MAPpin               = A3;   // set input pin for the MAP sensor

const int   Turbopin                           = 3;    // set output pin for the solenoid

const int   startPWM                             = 180;
int   PWMstep                        = 1;    // Instead of increasing 1 by 1
const int   PWMmin                          = 115;   
const int   PWMmax                          = 245;
int PWM =115;   

int   inputMaxBoostValue                    = 0;
float   UserBoost                    = 0.50;    // potentiometer input variable
//float MaxBoostValue                         = 0.50; // potentiometer value store
float BoostHardLimit                    = 1.09; // maximum value allowed (will be 1.50)

//const float boostTolerance                        = 0.05; // Tolerance to ignore action inside loop
float startupMAPReading                     = 0.00; // MAP reading at startup to store zero bar value
float MAP                           = 0.00;
int VMAP =0;
float nowMAP                        = 0.00;
float deltaMAP                        = 0.00;


int   inputAnalogVRSensorValue              = 0;    // VR sensor input variable
int   RPM                        = 0;
int   previousRPMSensorValue                = 0;
bool  RPMClimbing                           = false;
int   RPMChangeUnderLoadPerCycleTolerance   = 10;   // Needs testing with serial.print to find a good value

int   maxDutyCyclePerRPMVolts[5]            = { };  // RPM Reference table
int   maxDutyCyclePerRPM[5]                 = { };  // RPM Reference table

void setup() {
  
   pinMode(Turbopin, OUTPUT);// declare the solenoidPin as an OUTPUT:
   pinMode(VRpin, INPUT);
   pinMode(MAPpin, INPUT);
   pinMode(BoostSelectPin, INPUT);

  
  startupMAPReading = analogRead(MAPpin);
  
  Serial.begin(9600);
}

void loop() {
   UserBoost=1.50;
   MAP=0.51;
   nowMAP=0.51;
   RPM=700;
    //inputMaxBoostValue = analogRead(BoostSelectPin);  //Gets maxBoostValue from pot
  //UserBoost = (inputMaxBoostValue / 1023.00) + 0.50;
  for(int y=0;y<100;y++) {
    VMAP=analogRead(MAPpin);
    //MAP=findBoostFromVoltage(VMAP);
    //RPM = fetchRPM();
    if (MAP >= BoostHardLimit  ){
       PWM=0;
      analogWrite(Turbopin, PWM);
      Serial.println("Overboost Hard Limit");
      }
    if (MAP >= UserBoost  ){
       PWM=PWMmin;
      analogWrite(Turbopin, PWM);
      }
    if (RPM <= 615){
      PWM=PWMmin;
      analogWrite(Turbopin, PWM);
      }
    else
    {
     if (MAP > UserBoost  ){
       PWM=PWMmin;
      analogWrite(Turbopin, PWM);
      
      Serial.println("Overboost user Limit");
      }
      if (PWM < startPWM && MAP < 0.10  ){
       PWM=startPWM;
      analogWrite(Turbopin, PWM);
      Serial.println("Start PWM");
      }
      delay(50);
    //VMAP=analogRead(MAPpin);
    //nowMAP=findBoostFromVoltage(VMAP);
    deltaMAP = (UserBoost - nowMAP);
    int MAXrange;
    MAXrange=UserBoost*100;
    Serial.println(MAXrange);
    Serial.println(deltaMAP);
    int intDeltaMAP;
    intDeltaMAP=deltaMAP*100;
    int range = map(intDeltaMAP, 1, MAXrange, 0, 3);
    Serial.println(intDeltaMAP);
    Serial.println(range);
      switch (range) {
    case 0:    // Perto Do Maximo abir
      PWMstep=30;
      PWM=PWM-PWMstep;
      analogWrite(Turbopin, PWM);
      Serial.println("case0");
      break;
    case 1:    // Perto Do Maximo abir mas menos
      PWMstep=10;
      PWM=PWM-PWMstep;
      analogWrite(Turbopin, PWM);
      Serial.println("case1");
      break;
    case 2:    // longe do maximo fechar
      PWMstep=10;
      PWM=PWM+PWMstep;
      analogWrite(Turbopin, PWM);
      Serial.println("case2");
      break;
    case 3:    // muito longe do maximo fechar a seria
      PWMstep=30;
      PWM=PWM+PWMstep;
      analogWrite(Turbopin, PWM);
      Serial.println("case3");
      break;
  }
    
      
      
      }
    
    
    
  }
}


float findBoostFromVoltage(int voltage) {
  int tempVoltage = (voltage - startupMAPReading);
  float boost = (tempVoltage / 400.00);
  return boost;
}
int fetchRPM() {
  
  int tempVR = 0;
  int currentTempVR = 0;

  // TODO: Tentar o numero mais baixo possivel ao ralenti
  for(int z=0;z<250;z++) { // Corre x para apanhar sempre o topo da onda
    currentTempVR = analogRead(VRpin);
    if(currentTempVR > tempVR) tempVR = currentTempVR;
  }

  return tempVR;
}
