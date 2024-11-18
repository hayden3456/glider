#include "FastIMU.h"
#include <Wire.h>

#define IMU_ADDRESS 0x68    //Change to the address of the IMU
#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU6050 IMU;               //Change to the name of any supported IMU! 

unsigned long startTime = 0;
unsigned long endTime = 0;
float gPitch = 0.0;
float gRoll = 0.0;
float aPitch = 0.0;
float aRoll = 0.0;
float pitch = 0.0;
float roll = 0.0;
float yaw = 0.0;
float alpha = 0.98;
float nTime = 0.0;
float aVec = 0.0;
float x = 0.0;
// Currently supported IMUS: MPU9255 MPU9250 MPU6886 MPU6500 MPU6050 ICM20689 ICM20690 BMI055 BMX055 BMI160 LSM6DS3 LSM6DSL QMI8658

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
GyroData gyroData;
MagData magData;

void setup() {
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
  
#ifdef PERFORM_CALIBRATION
  Serial.println("FastIMU calibration & data example");
  delay(5000);
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib.gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib.gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib.gyroBias[2]);
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
#endif

  err = IMU.setGyroRange(2000);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  err = IMU.setAccelRange(8);       //THESE TWO SET THE GYRO RANGE TO ±2000 DPS AND THE ACCELEROMETER RANGE TO ±8g
  
  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
}

void loop() {
  startTime = micros();
  IMU.update();
  IMU.getAccel(&accelData); // Reads acceleration in g/s^2. g's referring to increments of the gravitational constant
  IMU.getGyro(&gyroData); // Reads instantaneous angular velocity in deg/s

  //convert instant velo to meters/sec (because we want nice units :)

  accelData.accelX = accelData.accelX*9.81;
  accelData.accelY = accelData.accelY*9.81;
  accelData.accelZ = accelData.accelZ*9.81;

  // calculate pitch and roll based on the accelerometer

  aRoll = atan(accelData.accelX/(sqrt(accelData.accelY * accelData.accelY + accelData.accelZ * accelData.accelZ)))*180.0/3.14159;
  aPitch = atan(accelData.accelY/(sqrt(accelData.accelX * accelData.accelX + accelData.accelZ * accelData.accelZ)))*180.0/3.14159;

  endTime = micros();

  // calculate pitch and roll from the Gyroscope

  nTime = (endTime - startTime)/1000000.0;
  gPitch = pitch + gyroData.gyroX * nTime * 3.0;
  gRoll = roll + gyroData.gyroY * nTime *3.0;

  // Adjust in case of Yaw
  gPitch -= gRoll * sin(gyroData.gyroZ*nTime*3.0*(3.14159/180.0));
  gRoll += gPitch * sin(gyroData.gyroZ*nTime*3.0*(3.14159/180.0));

  // simple complimentary filter. Hi = Gyro, low = Acc

  pitch = gPitch*0.98 + aPitch*0.02;
  yaw += gyroData.gyroZ*nTime*3.0;
  roll = gRoll*0.98 + aRoll*0.02;
  Serial.println(roll);
  
}