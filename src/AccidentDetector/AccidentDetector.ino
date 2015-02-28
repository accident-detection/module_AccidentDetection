//1. Library
#include "Wire.h"
#include "HMC5883L.h"

//2. Global variables
HMC5883L compass;
float xv, yv, zv;
float xold, yold, zold;

//3. Pinns
#define triggerInput_front 7
#define echoOutput_front 6
#define triggerInput_back 4
#define echoOutput_back 3
#define critical_distance 20
#define critical_gyro_up 1.20
#define critical_gyro_down 0.80

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass = HMC5883L();
  setupHMC5883L();
  pinMode(triggerInput_front, OUTPUT);
  pinMode(echoOutput_front, INPUT);
  pinMode(triggerInput_back, OUTPUT);
  pinMode(echoOutput_back, INPUT);;
}

void loop() {
  int accidentStatus = 0;
  accidentStatus = AccidentDetector();
  switch (accidentStatus) {
    case 0:
      Serial.println("Ok!");
      break;
    case 1:
      Serial.println("Distance!");
      break;
    case 2:
      Serial.println("Gyro!");
      break;
    case 3:
      Serial.println("Accident!");
      break;
  }
}

int AccidentDetector() {
  bool statusDistanceFront, statusDistanceBack;
  bool gyroWarning;
  bool distanceWarning;

  //Polling Gyroscope and Distance sensors
  gyroWarning = ReadGyro();
  statusDistanceFront = ReadDistanceFront();
  statusDistanceBack = ReadDistanceBack();

  //Encoding the status word
  distanceWarning = statusDistanceFront || statusDistanceBack;
  if (distanceWarning == false) {
    if (gyroWarning == false)
      return 0; //GD = 00
    else
      return 2; //GD = 10
  }
  else {
    if (gyroWarning == true)
      return 3; //GD = 11
    else
      return 1; //GD = 01
  }
}

bool ReadDistanceFront()
{
  long duration_front, distance_front;

  digitalWrite(triggerInput_front, LOW);
  digitalWrite(triggerInput_front, HIGH);
  digitalWrite(triggerInput_front, LOW);

  duration_front = pulseIn(echoOutput_front, HIGH);
  distance_front = (duration_front / 2.) / 29.1;
  return (distance_front < (long)critical_distance);
}

bool ReadDistanceBack()
{
  long duration_back, distance_back;

  digitalWrite(triggerInput_back, LOW);
  digitalWrite(triggerInput_back, HIGH);
  digitalWrite(triggerInput_back, LOW);

  duration_back = pulseIn(echoOutput_back, HIGH);
  distance_back = (duration_back / 2.) / 29.1;
  return (distance_back < (long)critical_distance);
}

bool ReadGyro() {
  getHeading();

  //Algorithm for determination of critical gyro change
  bool gyroChange = false;
  if (abs(xv) > abs(xold * critical_gyro_up) ||
      abs(xv) < abs(xold * critical_gyro_down) ||
      abs(yv) > abs(yold * critical_gyro_up) ||
      abs(yv) < abs(yold * critical_gyro_down) ||
      abs(zv) > abs(zold * critical_gyro_up) ||
      abs(zv) < abs(zold * critical_gyro_down)
     ) {
    gyroChange = true;
  }

  xold = xv;
  yold = yv;
  zold = zv;

  return gyroChange;
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

float calibrated_values[3];

void transformation(float uncalibrated_values[3]) {
  double calibration_matrix[3][3] =
  {
    { 1.078, 0.009, 0.003 },
    { 0.014, 1.073, -0.162 },
    { 0.038, 0.009, 1.216 }
  };

  double bias[3] =
  {
    -175.886,
    -190.091,
    57.551
  };

  for (int i = 0; i < 3; ++i)
    uncalibrated_values[i] = uncalibrated_values[i] - bias[i];

  float result[3] = { 0, 0, 0 };

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      result[i] += calibration_matrix[i][j] * uncalibrated_values[j];

  for (int i = 0; i < 3; ++i)
    calibrated_values[i] = result[i];
}
