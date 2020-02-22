#include <Wire.h>
#include "main.h"

void i2c_init()
{
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
}
void timer1_init()
{
    // set up timer with prescaler = 8
    TCCR1B |= (1 << CS11);
    TCNT1 = 45536; //10ms
    //TCNT1 = 51536; //7ms
    //TCNT1 = 57536; //4ms
    // enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);
    sei();
}
void setup() 
{
  
  i2c_init();
  timer1_init();
  pinMode(LED_pin,OUTPUT);
  Serial.begin(9600);
  //calculate_IMU_error(); // Call this function if you need to get the IMU error values for your module
 
}
void read_accel()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / ACCEL_SCALE_2G ; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / ACCEL_SCALE_2G ; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / ACCEL_SCALE_2G ; // Z-axis value
  //rms_a=pow((pow(AccX,2)+pow(AccY,2)+pow(AccZ,2)),0.5);
}

void read_gyro()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); 
  GyX = (Wire.read() << 8 | Wire.read()) / GYRO_SCALE_250; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyY = (Wire.read() << 8 | Wire.read()) / GYRO_SCALE_250;
  GyZ = (Wire.read() << 8 | Wire.read()) / GYRO_SCALE_250;
}
void print_all()
{
  Serial.print("AX: ");
  Serial.print(AccX);
  Serial.print("\tAY: ");
  Serial.print(AccY);
  Serial.print("\tAZ: ");
  Serial.print(AccZ);

  Serial.print("\tGX: ");
  Serial.print(GyX);
  Serial.print("\tGY: ");
  Serial.print(GyY);
  Serial.print("\tGZ: ");
  Serial.println(GyZ);
}

void column_avg()
{
  averageAccX = valuesAccX / totalValues; 
  averageAccY = valuesAccY / totalValues; 
  averageAccZ = valuesAccZ / totalValues;

  averageGyX = valuesGyX / totalValues; 
  averageGyY = valuesGyY / totalValues; 
  averageGyZ = valuesGyZ / totalValues;
  
}
void reset_all()
{
  valuesAccX = 0; valuesAccY = 0; valuesAccZ = 0;
  valuesGyX = 0; valuesGyY = 0; valuesGyZ = 0;
  totalValues = 0;
  calculate_flag = 1;
  if(n%100==0) digitalWrite(LED_pin,LOW);
}
void handle_data()
{
  valuesAccX += AccX; valuesAccY += AccY; valuesAccZ += AccZ;
  valuesGyX += GyX;   valuesGyZ += GyZ;   valuesGyZ += GyZ; 
}


void predict()
{ 
  a =  1.793448955856741 ;
  b =  -2.2175591848784673 ;
  c =  0.13261748920926242 ;
  d =  0.00044467518841145984 ;
  e =  0.005613919363039376 ;
  f =  0.04211877060872497 ;
  intercept =  0.48888174909240767 ;
  z = a*averageAccX + b*averageAccY + c*averageAccZ + d*averageGyX + e*averageGyY + f*averageGyZ + intercept;
  prediction = 1/(1+exp(-z));
  
  //Serial.print("z = ");Serial.print(z);Serial.print("\t");
 // Serial.print("prediction = ");Serial.println(prediction);
  
  if(prediction>LRThreshold) inflate_airbags();
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 45536;  //10ms
  //TCNT1 = 51536; //7ms
  //TCNT1 = 57536; //4ms
  calculate_flag=0;
  n++;
}

void inflate_airbags()
{
  digitalWrite(LED_pin,HIGH);
}

void task_scheduler()
{
  while(calculate_flag == 1) //calculate_flag becomes 0 after 10ms //Write an ISR for this. 
  {
    totalValues++;
    read_accel();
    read_gyro();
    handle_data();  
  }
  column_avg();
  predict();
  reset_all();
}

void loop() 
{
  task_scheduler();
  print_all();
}
