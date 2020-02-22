#include <Wire.h>
const int MPU = 0x68; // MPU6050 I2C address
#define PI 3.1415926535897932384626433832795

int16_t n = 1;


// roll,roll_prev,pitch,pitch_prev,yaw;
float AccX, AccY, AccZ , rms_a;
float GyX, GyY, GyZ , rms_gy;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
//float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
int prev_n = -100;
float Accel_X, Accel_Y, Accel_Z , Gyro_X, Gyro_Y, Gyro_Z;

void setup() {
  
  Serial.begin(9600);
  //Serial.begin(128000); // opens serial port, sets data rate to 128000 bps
  //Serial.println("CLEARDATA");
  //Serial.println("LABEL,Time,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z");
  Wire.begin();                      // Initialize comunication
  
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  /*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  */
  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  delay(20);
  //pinMode(43,OUTPUT);
  //pinMode(45,OUTPUT);
  //digitalWrite(43,HIGH);
  //digitalWrite(45,HIGH);
}
void loop() {

  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read())/ 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read())/ 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read())/ 16384.0; // Z-axis value
  rms_a=pow((pow(AccX,2)+pow(AccY,2)+pow(AccZ,2)),0.5);

  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); 

  GyX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  
  /*Serial.print("AccX: ");
  Serial.print(AccX);
  Serial.print("\tAccY: ");
  Serial.print(AccY);
  Serial.print("\tAccZ: ");
  Serial.print(AccZ);*/

 
  /*Serial.print("\tGyX: ");
  Serial.print(GyX);
  Serial.print("\tGyY: ");
  Serial.print(GyY);
  Serial.print("\tGyZ: ");
  Serial.print(GyZ);*/




  
  rms_gy=pow((pow(GyX,2)+pow(GyY,2)+pow(GyZ,2)),0.5);
  Serial.print(AccX); Serial.print(",");Serial.print(AccY);Serial.print(",");Serial.print(AccZ);Serial.print(","); Serial.print(GyX);Serial.print(","); Serial.print(GyY);Serial.print(","); Serial.println(GyZ);
  
  delay(10);
  
}
