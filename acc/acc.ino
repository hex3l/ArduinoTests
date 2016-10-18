//-----------
//  MPU6050
//-----------

#include <Wire.h>
#define MPU 0x68  // I2C address of the MPU-6050

double AcX,AcY,AcZ;
int angle, led;

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

  angle = map(AcY, -16000, 16000, 0, 360);

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
  delay(100);
}

void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}

//Funzione per l'acquisizione degli assi X,Y,Z del MPU6050
void FunctionsMPU(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}
