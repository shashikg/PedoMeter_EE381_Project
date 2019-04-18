#include <SparkFunMPU9250-DMP.h>

MPU9250_DMP imu;

float a[3];
int i;
int stepC = 0;
int flagD = 0;
int flagU = 0;

void setup() 
{
  Serial.begin(9600);
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
//      Serial.println("Unable to communicate with MPU-9250");
//      Serial.println("Check connections, and try again.");
//      Serial.println();
      delay(5000);
    }
  }

  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(2000);
  imu.setAccelFSR(2);
  imu.setLPF(5);
  imu.setSampleRate(10); 
  imu.setCompassSampleRate(10);

  for(i=0;i<3;i++){
    a[i] = 1;
  }

  Serial.println(stepC);
}

void loop() 
{
  if ( imu.dataReady() )
  {
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    printIMUData();
  }
}


void printIMUData(void)
{  
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);

  float accm = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);

  for( i =0; i<2;i++){
    a[i] = a[i+1];
  }
  a[2] = accm;

  float accmf = 0;

  // time averaging over 3 samples to remove noisy data.
  for(i=0;i<3;i++){
    accmf = accmf + a[i];
  }
  accmf = accmf/3;

  // detection valley's inacceleration data.
  if (accmf>1.1){    
    if (flagD==1){
      stepC++;
      flagD = 0;
      Serial.println(stepC);
    }    
  }else if(accmf<0.85){
    flagD = 1;
  }  
}
