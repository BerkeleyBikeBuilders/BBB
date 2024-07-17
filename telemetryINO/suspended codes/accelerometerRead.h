#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

// SETUP
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h> 
Adafruit_MPU6050 mpu;


// Declare Variables for IMU
float accel_xA, accel_yA, accel_zA;
float x_angle, y_angle;
float squareX, squareY;
float gyro_angle_x, gyro_angle_y, gyro_angle_z;
float currentTime, previousTime, dt;
float accX, accY, accZ;
float pitch, roll, yaw;
float gyroX, gyroY, gyroZ;

float yawSlope = 0;

float pitch_offset = 0;
float roll_offset = 0;
float yaw_offset = 0;
// SETUP


void create_acc(int SDA, int SCL){
  Wire.begin(SDA, SCL); // declares the SDA and SCL pins.
  
  while (!mpu.begin()){
    if (mpu.begin()){
        Serial.println("MPU6050 Found!");
        break;
      }
    Serial.println("MPU6050 Not Found");
    delay(100);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);

  zero(); // initialise the roll, pitch, yaw to 0... (which may not be nessesary right here)
  find_yawSlope(); // for drift correction
}

/**
void loop() {
  pitch = read_pitch();
  roll = read_roll();
  yaw = read_yaw();

  Serial.print("yaw:");
  Serial.print(yaw);
}
*/

// function for reading the pitch
float read_pitch(){
  previousTime = currentTime;
  currentTime = millis();
  dt = (currentTime - previousTime) / 1000;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  gyroX = g.gyro.x - 1.2;

  // Calculte the pitch roll and yaw of acceleramator and gyroscope
  squareX = sqrt(pow(accX, 2) + pow(accZ, 2));

  if (squareX == 0) {
    squareX = 0.1;
  }
  accel_xA = atan(accY / squareX) * 180 / PI;
  gyro_angle_x = pitch + gyroX * dt;

  // Complimentary Filter
  pitch = 0.96 * gyro_angle_x + 0.04 * accel_xA;  //if needed the numeric values can be changed in order to change the wieght of the filter values 0>x>1

  return pitch - pitch_offset;
}

float read_roll(){
  previousTime = currentTime;
  currentTime = millis();
  dt = (currentTime - previousTime) / 1000;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  // gyroX = g.gyro.x - 1.2;
  gyroY = g.gyro.y + 1.3;
  // gyroZ = g.gyro.z;

  // Calculte the pitch roll and yaw of acceleramator and gyroscope
  squareY = sqrt(pow(accY, 2) + pow(accZ, 2));
  if (squareY == 0) {
    squareY = 0.1;
  }
  accel_yA = atan(accX / squareY) * 180 / PI;
  gyro_angle_y = roll + gyroY * dt;

  // Complimentary Filter
  roll = 0.96 * gyro_angle_y + 0.04 * accel_yA; // if needed the numeric values can be changed in order to change the wieght of the filter values 0>x>1

  return roll - roll_offset;
}

float read_yaw(){
  previousTime = currentTime;
  currentTime = millis();
  dt = (currentTime - previousTime) / 1000;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accZ = a.acceleration.z;
  gyroX = g.gyro.x - 1.2;
  gyroY = g.gyro.y + 1.3;
  gyroZ = g.gyro.z * 180 + 1.285;

  // Calculte the yaw with the yawSlope subtracted
  yaw = yaw + (gyroZ - yawSlope) * dt;

  return yaw - yaw_offset;
}

void zero(){
  
  pitch_offset = 0;
  roll_offset = 0;
  yaw_offset = 0;

  float error = 0.15;
  float mag_accel;
  int sample_count = 0;

  while (sample_count < 100){
    // records 100 measurements while the bike is stationary.

    if (abs(mag_accel - 9.81) < error){
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      accX = a.acceleration.x;
      accY = a.acceleration.y;
      accZ = a.acceleration.z;

      mag_accel = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));

      // temporarily stores the measured values in the offsets (to be averaged out later) 
      pitch_offset += read_pitch();
      roll_offset += read_roll();
      yaw_offset += read_yaw();

      sample_count += 1;
    } else{
      // if the bike moves at any time during calibration, restart it.

      Serial.print("detected movement, restarting calibration");
      return zero();
    }
  }

  // averaged out the measurements to find the actual offsets
  pitch_offset /= 100;
  roll_offset /= 100;
  yaw_offset /= 100;
  Serial.print("done calibration");
}

void find_yawSlope(){
  /**
  uses Simple Linear Regression
  */

  // summations needed for calculation:
  float sum_TimeYaw = 0;
  float sum_Time = 0;
  float sum_Time_squared = 0;
  float sum_Yaw = 0;

  float test_yaw;
  float test_time;

  yawSlope = 0; // reset the old value
  previousTime = millis(); // inititalise the starting time
  
  for (int i = 1; i <= 100; i++){
    // measure and sum 100 times
    test_time = millis() + 1; // adding 1 to better match the 'currentTime' from read-yaw();
    test_yaw = read_yaw();
    
  
    sum_TimeYaw += test_yaw * test_time;
    sum_Time += test_time;
    sum_Time_squared += test_time * test_time;
    sum_Yaw += test_yaw;
  }
  
  // using the formula for gradient:
  yawSlope = (100*sum_TimeYaw - sum_Time*sum_Yaw) / 
             (100*sum_Time_squared - pow(sum_Time, 2));
}

#endif