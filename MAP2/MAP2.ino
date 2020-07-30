int   inputAnalogMAPSensorPin        = A3;     // set input pin for the MAP sensor
int   inputAnalogMAPSensorValue      = 0;      // MAP sensor input variable
float MAPSensorValue                 = 0.97;
float temp1CalcAnalogMAPSensorValue  = 0;
float temp2CalcAnalogMAPSensorValue  = 0;

int   MAPReferenceVoltages[24]       = { };   // MAP Reference table
float MAPReferenceBoost[24]          = { };   // MAP Reference table

void setup() {
  // put your setup code here, to run once:
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
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  inputAnalogMAPSensorValue = analogRead(inputAnalogMAPSensorPin);    // fetch MAP
  MAPSensorValue = findBoostFromVoltage(inputAnalogMAPSensorValue);

  Serial.print("MAP(Volts):         ");
  Serial.println(inputAnalogMAPSensorValue);
  Serial.print("MAP(Bar):         ");

  delay(500);
}

float findBoostFromVoltage(int voltage) {
  float boost = 0.50;

  if(voltage > 1005) {
    Serial.println("Error: Overboost !!!");
    //return MaxBoostValueLimit;
  }
  
  for(int x=0;x<sizeof(MAPReferenceVoltages);x++) {
    if(MAPReferenceVoltages[x] <= voltage) {
      boost = MAPReferenceBoost[x];
    } else if (MAPReferenceVoltages[x] > voltage) {
      return  boost;
    } else {
      Serial.println("Error: Boost off range !!!");
      //return MaxBoostValueLimit; // Is this a good idea???
    }
  }
  
  // Se falhar algo devolve o máximo
  //return MaxBoostValueLimit;
}
