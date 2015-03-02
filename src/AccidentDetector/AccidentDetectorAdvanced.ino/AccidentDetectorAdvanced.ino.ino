//1. Library
#include "Wire.h"
#include "HMC5883L.h"

//2. Global variables
HMC5883L compass;
float xv, yv, zv;
float xold, yold, zold;

//3. Pinns
#define triggerInput_front 4
#define echoOutput_front 3
#define triggerInput_back 7
#define echoOutput_back 6
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
  int accidentState = 0;
  Serial.println();
  AccidentDetector(&accidentState);
  Serial.println(accidentState);
  switch (accidentState) {
    case 1:
      Serial.println("201 - Back sensor!");
      break;
    case 2:
      Serial.println("202 - Front sensor!");
      break;
    case 3:
      Serial.println("203 - Both sensors!");
      break;
    case 4:
      Serial.println("204 - Gyro reacted!");
      break;
    case 5:
      Serial.println("205 - BACK HIT!");
      Serial.println("205 - BACK HIT!");
      Serial.println("205 - BACK HIT!");
      break;
    case 6:
      Serial.println("206 - FRONT HIT!");
      Serial.println("206 - FRONT HIT!");
      Serial.println("206 - FRONT HIT!");
      break;
    case 7:
      Serial.println("207 - HIT WHILE SURROUNDED!");
      Serial.println("207 - HIT WHILE SURROUNDED!");
      Serial.println("207 - HIT WHILE SURROUNDED!");
      break;
    default:
      Serial.println("200 - Everything ok!");
      break;
  }
}

void AccidentDetector(int* state) {

  bool distanceFront, distanceBack;
  bool gyroWarning;
  int newState;

  //Polling Gyroscope and Distance sensors
  gyroWarning = ReadGyro();
  distanceFront = ReadDistanceFront();
  distanceBack = ReadDistanceBack();

  //Calculating new state
  if (gyroWarning == false) {	//0XX
    if (distanceFront == false) {	//00X
      if (distanceBack == false)	//000
        newState = 0;
      else //001
        newState = 1;
    }
    else {	//01X
      if (distanceBack == false)	//010
        newState = 2;
      else //011
        newState = 3;
    }
  }
  else {	//1XX
    if (distanceFront == false) {	//10X
      if (distanceBack == false)	 //100
        newState = 4;
      else  //101
        newState = 5;
    }
    else { //11X
      if (distanceBack == false)	 //110
        newState = 6;
      else  //111
        newState = 7;
    }
  }

  switch (newState) {
    case 5:
      *state = 5;
      break;
    case 6:
      *state = 6;
      break;
    case 7:
      *state = 7;
      break;
    default:
      *state = newState;
      break;
  }
  return;
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

