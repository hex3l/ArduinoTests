//-----------
//  MPU6050
//-----------

#include <Wire.h>
#define MPU 0x68  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int angle, led;
int Pitch;

void setup(){
  Serial.begin(9600);
  init_MPU(); // Inizializzazione MPU6050

  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(11,HIGH);
  led = 11;
}
 
void loop()
{
  FunctionsMPU(); // Acquisisco assi AcX, AcY, AcZ.
  Pitch = FunctionsPitchRoll(AcY, AcX, AcZ);  //Calcolo angolo Pitch
  
  int angle = map(Pitch, -90, 90, 360, 0);


  Serial.print("Pitch: "); Serial.print(Pitch);
  Serial.print("\t");

  if(angle > 20 || angle < -20){
    if(angle > 20){
      if(led < 13){
        digitalWrite(led, LOW);
        led++;
        digitalWrite(led, HIGH);
      }
    }else{
      if(led > 9){
        digitalWrite(led, LOW);
        led--;
        digitalWrite(led, HIGH);
      }
    }
  }
  delay(500);
}

void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}

//Funzione per il calcolo degli angoli Pitch e Roll
double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  
  Value = atan2(DatoA, DatoB);
  Value = Value * 180/3.14;
  
  return (int)Value;
}

//Funzione per l'acquisizione degli assi X,Y,Z del MPU6050
void FunctionsMPU(){
    Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
