#include "Wire.h"
#include "HMC5883L.h"

HMC5883L compass; //Copy the folder "HMC5883L" in the folder "C:\Program Files\Arduino\libraries" and restart the arduino IDE.

float xv, yv, zv;
float xold, yold, zold;

void setup()
{   
  Serial.begin(9600);
  Wire.begin();  
  compass = HMC5883L();  
  setupHMC5883L();       
}

void loop()
{
  getHeading();

  Serial.flush(); 
  Serial.print(xv); 
  Serial.print(",");
  Serial.print(yv);
  Serial.print(",");
  Serial.print(zv);
  Serial.println();

  if(abs(xv) > abs(xold*1.15) || abs(xv) < abs(xold * 0.85))
    Serial.print(" X ");
  if(abs(yv) > abs(yold*1.15) || abs(yv) < abs(yold * 0.85))
    Serial.print(" Y ");
  if(abs(zv) > abs(zold*1.15) || abs(zv) < abs(zold * 0.85))
    Serial.print(" Z ");
  Serial.println();
  xold = xv;
  yold = yv;
  zold = zv;

  delay(100); 
} 

void setupHMC5883L()
{  
  compass.SetScale(0.88);
  compass.SetMeasurementMode(Measurement_Continuous);
}

void getHeading()
{ 
  MagnetometerRaw raw = compass.ReadRawAxis();
  xv = (float)raw.XAxis;
  yv = (float)raw.YAxis;
  zv = (float)raw.ZAxis;
}

//arduino example code for getting the calibrated magnetometer data 
//calibrated_values[3] is the global array where the calibrated data will be placed
//calibrated_values[3]: [0]=Xc, [1]=Yc, [2]=Zc
float calibrated_values[3];   
//transformation(float uncalibrated_values[3]) is the function of the magnetometer data correction 
//uncalibrated_values[3] is the array of the non calibrated magnetometer data
//uncalibrated_values[3]: [0]=Xnc, [1]=Ync, [2]=Znc
void transformation(float uncalibrated_values[3])    
{
  //calibration_matrix[3][3] is the transformation matrix
  //replace M11, M12,..,M33 with your transformation matrix data
  double calibration_matrix[3][3] = 
  {
    {
      1.078, 0.009, 0.003        }
    ,
    {
      0.014, 1.073, -0.162        }
    ,
    {
      0.038, 0.009, 1.216        }  
  };
  //bias[3] is the bias
  //replace Bx, By, Bz with your bias data
  double bias[3] = 
  {
    -84.857,
    -94.661,
    -60.051
  };  
  //calculation
  for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {
    0, 0, 0    };
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
  for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}


