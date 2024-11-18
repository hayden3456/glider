#include <MPU9250.h>
#include <Wire.h>

MPU9250 IMU(Wire , 0x68);


float xAcc = 0;
float yAcc = 0;
float zAcc = 0;

float xRotAcc = 0;
float yRotAcc = 0;
float zRotAcc = 0;

void setup() {
  Serial.begin(9600);
  IMU.begin();
  IMU.calibrateAccel();
  IMU.calibrateGyro();
}

void loop() {
  IMU.readSensor();
  // xAcc = IMU.getAccelX_mss();
  // yAcc = IMU.getAccelY_mss();
  // zAcc = IMU.getAccelZ_mss();
  // Serial.print("Accelerometer X Axis: ");
  // Serial.print(xAcc, 3);
  // Serial.print("          Accelerometer Y Axis: ");
  // Serial.print(yAcc, 3);
  // Serial.print("          Accelerometer Z Axis: ");
  // Serial.println(zAcc, 3);
  xRotAcc = -(((IMU.getGyroX_rads()) * 360)/(3.14159 * 2));
  yRotAcc = (((IMU.getGyroY_rads()) * 360.0)/(3.14159 * 2));
  zRotAcc = -(((IMU.getGyroZ_rads()) * 360.0)/(3.14159 * 2));
  Serial.print("Gyro X Axis: ");
  Serial.print(xRotAcc, 3);
  Serial.print("          Gyro Y Axis: ");
  Serial.print(yRotAcc, 3);
  Serial.print("          Gyro Z Axis: ");
  Serial.println(zRotAcc, 3);
  // Serial.println("****** Break *******");
  // Serial.println("");
  // Serial.println("");
  // Serial.println("");
  // Serial.println("");
  delay(200);

}
