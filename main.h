#ifndef _MAIN_H    // Put these two lines at the top of your file.
#define _MAIN_H    // (Use a suitable name, usually based on the file name.)

#define ACCEL_SCALE_2G 16384.0
#define GYRO_SCALE_250  131.0
#define PI 3.1415926535897932384626433832795
#define LRThreshold 0.5
#define LED_pin 13

//FUNCTION PROTOTYPES
void i2c_init();
void read_accel();
void read_gyro();
void print_all();

volatile int calculate_flag = 1;
const int MPU = 0x68; // MPU6050 I2C address
int16_t n = 1;
// roll,roll_prev,pitch,pitch_prev,yaw;
double AccX, AccY, AccZ , rms_a;
double GyX, GyY, GyZ , rms_gy;
double valuesAccX,valuesAccY,valuesAccZ,valuesGyX,valuesGyY,valuesGyZ;
double averageAccX,averageAccY,averageAccZ,averageGyX,averageGyY,averageGyZ;
double a,b,c,d,e,f,intercept,z,prediction;
int totalValues;

#endif // _HEADERFILE_H    // Put this line at the end of your file.
