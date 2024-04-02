#include <Arduino.h>
#include <UltrasonicSensorK.h>
#include <MotorL293DControllerK.h>
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

const char* ssid = "";
const char* password = "";

#define LED 2
#define TRIGPIN1 15
#define ECHOPIN1 14
#define TRIGPIN2 19
#define ECHOPIN2 21
#define TRIGPIN3 22
#define ECHOPIN3 35
#define TRIGPIN4 25
#define ECHOPIN4 26
#define TRIGPIN5 27
#define ECHOPIN5 32
#define TRIGPIN6 33
#define ECHOPIN6 34

#define MOTOR1FOWARDPIN 17
#define MOTOR1BACKWARDPIN 18
#define MOTOR1ENABLEPIN 13
#define MOTOR2FOWARDPIN 5
#define MOTOR2BACKWARDPIN 4
#define MOTOR2ENABLEPIN 16

#define MINMOTORVALUE 40
#define MAXMOTORVALUE 255


//------------------------------------------------------------------------------//
//define the server
WiFiServer server(80);

//define ultrasonic sensors
UltrasonicSensorK UltraS1(TRIGPIN1, ECHOPIN1);
UltrasonicSensorK UltraS2(TRIGPIN2, ECHOPIN2);
UltrasonicSensorK UltraS3(TRIGPIN3, ECHOPIN3);
UltrasonicSensorK UltraS4(TRIGPIN4, ECHOPIN4);
UltrasonicSensorK UltraS5(TRIGPIN5, ECHOPIN5);
UltrasonicSensorK UltraS6(TRIGPIN6, ECHOPIN6);

//define 2 motors
MotorL293DControllerK motor1(MOTOR1FOWARDPIN, MOTOR1BACKWARDPIN, MOTOR1ENABLEPIN, 8, 30000);
MotorL293DControllerK motor2(MOTOR2FOWARDPIN, MOTOR2BACKWARDPIN, MOTOR2ENABLEPIN, 8, 30000);

//------------------------------------------------------------------------------//
//define functions used
void startWIFI();
float startEpoch(int numEpoch, unsigned long secondsEpoch, float* parameters, float amplifier, int v0);
float sensorFunction(float distance);
void applyMutation(float* parameters, float* returnArray, int epoch);
bool equivalentArray(float* a, float* b);

void clearBuffer(char* buffer);

//Global variables
int len = 4;


void inviaDati(char* dato) {
  Udp.beginPacket("192.168.1.7", 2025);
  Udp.print(dato);
  Udp.endPacket();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);


  UltraS1.begin();
  UltraS2.begin();
  UltraS3.begin();
  UltraS4.begin();
  UltraS5.begin();
  UltraS6.begin();

  //configure 2 motors
  motor1.begin();
  motor2.begin();
  motor1.setPWM(255);
  motor2.setPWM(255);

  digitalWrite(LED, HIGH);

  //connect wifi
  startWIFI();
}

int totEpoch = 60;
unsigned long secondsEpoch = 60;
int collisionCounterSens1 = 0, collisionCounterSens2 = 0;

void loop() {
  Serial.println("----------------------------");


  float bestScore = 0;
  float parameters[len];
  float bestParameters[len];

  /*parameters[0] = float(esp_random() % 100) / 100;  //here we are considering floats
  delay(10);
  parameters[1] = float(esp_random() % 100) / 100;
  delay(10);
  parameters[2] = float(esp_random() % 100) / 100;
  delay(10);
  parameters[3] = float(esp_random() % 100) / 100;*/
  parameters[0]=0;
  parameters[1]=1;
  parameters[2]=0;
  parameters[3]=1;



  int pos = 0;

  char buffer[100];
  sprintf(buffer, "\n SESSION STARTED \n");
  inviaDati(buffer);
  clearBuffer(buffer);

  buffer[100];
  sprintf(buffer, " >Starting epoch %d with configuration: a=%f b=%f c=%f d=%f", 0, parameters[0], parameters[1], parameters[2], parameters[3]);
  inviaDati(buffer);
  clearBuffer(buffer);
  float score = startEpoch(0, secondsEpoch, parameters, 90, 130);
  //copy the best scores with relative parameters
  bestScore = score;
  //Serial.println("dim "+(String)(sizeof(parameters)/sizeof(parameters[0])));
  for (int i = 0; i < len; i++) {
    bestParameters[i] = parameters[i];
  }
  buffer[100];
  sprintf(buffer, "new best score %f with configuration: a=%f b=%f c=%f d=%f", score, parameters[0], parameters[1], parameters[2], parameters[3]);
  inviaDati(buffer);
  clearBuffer(buffer);
  //apply mutation
  applyMutation(bestParameters, parameters, 0);
  for (int g = 0; g < len; g++) {
    Serial.println((String)bestParameters[g] + " = " + (String)parameters[g]);
  }

  motor1.stop();
  motor2.stop();
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);

  for (int i = 0; i < (totEpoch - 1); i++) {

    char buffer[100];
    sprintf(buffer, "\n >Starting epoch %d with configuration: a=%f b=%f c=%f d=%f", i + 1, parameters[0], parameters[1], parameters[2], parameters[3]);
    inviaDati(buffer);
    clearBuffer(buffer);
    //start epoch
    Serial.println("starting epoch " + (String)(i + 1));
    float score = startEpoch(i, secondsEpoch, parameters, 50, 130);  //last 130 with full battery
    //check the score and send data to PC
    if (score <= bestScore) {  //new best score found
      char buffer[100];
      sprintf(buffer, "new best score %f with configuration: a=%f b=%f c=%f d=%f ", score, parameters[0], parameters[1], parameters[2], parameters[3]);
      inviaDati(buffer);
      clearBuffer(buffer);
      bestScore = score;
      for (int h = 0; h < len; h++) {
        bestParameters[h] = parameters[h];
      }
    } else {  //just notifying the score
      char buffer[100];
      sprintf(buffer, "score %f with configuration: a=%f b=%f c=%f d=%f ", score, parameters[0], parameters[1], parameters[2], parameters[3]);
      inviaDati(buffer);
      clearBuffer(buffer);
    }
    //apply the mutation
    for (int g = 0; g < 4; g++) {
      Serial.println("from " + (String)bestParameters[g] + " a mutation may be applied  ");
    }
    applyMutation(bestParameters, parameters, i);
    for (int g = 0; g < len; g++) {
      Serial.println("after " + (String)bestParameters[g] + " = " + (String)parameters[g]);
    }

    motor1.stop();
    motor2.stop();
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    /*motor1.setPWM(255);
    motor2.setPWM(255);
    motor1.moveBackward();
    motor2.moveBackward();
    delay(1500);
    motor1.stop();
    motor2.stop();*/
  }
}

float startEpoch(int numEpoch, unsigned long secondsEpoch, float* parameters, float amplifier, int v0) {
  unsigned long startTime = millis();
  float score = 0;
  unsigned long fx1, fx2;
  int m1, m2;
  bool direction = 0;  //specify the direction of the robot(0=forward,1=backward)
  int collisionCounter = 0;
  int collisionAdder = 2;
  int flag, wall, collisionSensor1, collisionSensor2;
  int first = 0, changed = 1;
  float previous_d1, previous_d3;
  float previous_d4, previous_d6;
  first = 0;
  previous_d1 = 100;
  previous_d3 = 100;
  previous_d4 = 100;
  previous_d6 = 100;

  while (millis() - startTime <= secondsEpoch * 1000) {

    if (changed) {  //only when change direction
      flag = 0;
      wall = 0;
      collisionSensor1 = 0;
      collisionSensor2 = 0;
      first = 0;
      previous_d1 = 100;
      previous_d3 = 100;
      previous_d4 = 100;
      previous_d6 = 100;
      changed = 0;
    }

    if (!direction) {  //Forward
                       //measure distances
      float d1 = UltraS1.measureDistance();
      delay(150);
      float d3 = UltraS3.measureDistance();
      delay(30);


      //check the collision conditions
      if (d1 < 8 || d3 < 8) {
        flag = 0;
        if (d1 < 8) collisionSensor1 = 1;
        if (d3 > 8) collisionSensor2 = 1;
      } else {
        flag = 1;
        if (d1 >= 8) collisionSensor1 = 0;
        if (d3 >= 8) collisionSensor2 = 0;
      }
      if (abs(d1 - d3) < 5) wall = 1;
      else wall = 0;


      //track if the robot avoided an obstacle
      if (first == 0 && (d1 < 30 || d3 < 30) && wall == 0) {  //good for static obstacles
        first = 1;
      }
      if ((previous_d1 + 40 < d1 || previous_d3 + 40 < d3) && first == 1) {  //if we measure a distance greater than the previuos one it means we avoided an object
        first = 0;
        score = score - 1;  //update the score in better
        char buffer[100];
        sprintf(buffer, "debug sx %f -> %f; dx %f -> %f", previous_d1 , d1, previous_d3, d3);
        inviaDati(buffer);
        clearBuffer(buffer);
      }
      //apply the functions to identify the speed motor
      fx1 = (v0 + sensorFunction(d1, amplifier) * parameters[0] + sensorFunction(d3, amplifier) * parameters[3]);
      fx2 = (v0 + sensorFunction(d1, amplifier) * parameters[1] + sensorFunction(d3, amplifier) * parameters[2]);
      //collect the score
      /////score = score + 1/d1 + 1/d3;
      if (wall == 0 && flag == 0) {
        score = score + collisionAdder;  //in case of collision worsens the score
        collisionCounter++;
      }
      if (collisionSensor1) collisionCounterSens1++;
      if (collisionSensor2) collisionCounterSens2++;
      //set range constraint
      m1 = constrain(fx1, MINMOTORVALUE, MAXMOTORVALUE);
      m2 = constrain(fx2, MINMOTORVALUE, MAXMOTORVALUE);
      //set the motor speed
      motor1.setPWM(m1);
      motor2.setPWM(m2);



      previous_d1 = d1;
      previous_d3 = d3;


      if (flag == 1) {
        motor1.moveForward();
        motor2.moveForward();
      } else {
        motor1.stop();
        motor2.stop();
        direction = 1;
        flag = 0;
        changed = 1;
        delay(1000);
      }
    } else {  //Backward

      //measure distances
      float d4 = UltraS4.measureDistance();
      delay(150);
      float d6 = UltraS6.measureDistance();
      delay(30);
      if (d4 < 8 || d6 < 8) {
        flag = 0;
        if (d6 < 8) collisionSensor1 = 1;
        if (d4 > 8) collisionSensor2 = 1;
      } else {
        flag = 1;
        if (d6 >= 8) collisionSensor1 = 0;
        if (d4 >= 8) collisionSensor2 = 0;
      }
      if (abs(d4 - d6) < 5) wall = 1;
      else wall = 0;

      //track if the robot avoided an obstacle
      if (first == 0 && (d4 < 30 || d6 < 30) && wall == 0) {  //good for static obstacles
        first = 1;
      }
      /*char buffer[100];
      sprintf(buffer, "debug %f < %f", previous_d4 + 2 , d4);
      inviaDati(buffer);
      clearBuffer(buffer);*/               ///add more delay in measurements
      if ((previous_d4 + 40 < d4 || previous_d6 + 40 < d6) && first == 1) {  //if we measure a distance greater than the previuos one it means we avoided an object
        first = 0;
        score = score - 1;  //update the score in better
        char buffer[100];
        sprintf(buffer, "debug sx %f -> %f; dx %f -> %f", previous_d4, d4, previous_d6, d6);
        inviaDati(buffer);
        clearBuffer(buffer);
      }
      //apply the functions to identify the speed motor
      fx1 = (v0 + sensorFunction(d4, amplifier) * parameters[2] + sensorFunction(d6, amplifier) * parameters[1]);
      fx2 = (v0 + sensorFunction(d4, amplifier) * parameters[3] + sensorFunction(d6, amplifier) * parameters[0]);
      //collect the score
      /////score = score + 1/d4 + 1/d6;
      if (collisionSensor1) collisionCounterSens1++;
      if (collisionSensor2) collisionCounterSens2++;
      if (wall == 0 && flag == 0) {
        score = score + collisionAdder;  //in case of collision worsens the score
        collisionCounter++;
      }
      //set range constraint
      m1 = constrain(fx1, MINMOTORVALUE, MAXMOTORVALUE);
      m2 = constrain(fx2, MINMOTORVALUE, MAXMOTORVALUE);
      //set the motor speed
      motor1.setPWM(m1);
      motor2.setPWM(m2);

      previous_d4 = d4;
      previous_d6 = d6;


      if (flag == 1) {
        motor1.moveBackward();
        motor2.moveBackward();
      } else {
        motor1.stop();
        motor2.stop();
        direction = 0;
        flag = 0;
        changed = 1;
        delay(1000);
      }
    }
  }

  char buffer[100];
  sprintf(buffer, "collision counter: %d", collisionCounter);
  inviaDati(buffer);
  clearBuffer(buffer);

  return score;
}

float sensorFunction(float distance, float amplifier) {
  return pow(40 / (distance + 1), 14) * amplifier;
}

void applyMutation(float* a, float* b, int epoch) {
  Serial.println("dim interna " + (String)len);
  for (int j = 0; j < len; j++) {
    b[j] = a[j];
  }
  long pos;
  float temp[len];
  do {

    /*if (collisionCounterSens1 > collisionCounterSens2) {  //here update the genes of the parameters regarding the sensor with the most collisions
      pos = esp_random() % len / 2;
    } else {
      pos = (esp_random() % (len / 2)) + (len / 2);
    }*/
    pos = esp_random() % len;
    Serial.println("mutation applied at pos " + (String)pos);
    Serial.print("before " + (String)a[pos]);

    for (int j = 0; j < len; j++) {
      temp[j] = b[j];
    }

    //extended version
    //b[pos] = float(esp_random()%100)/100; //old mutation
    if (epoch < totEpoch / 2) {  //for the firs half of total epochs
      int rand = int(esp_random() % 5);
      switch (rand) {  //divide the interval into 3 ranges (quantization-like)  smaller search space
        case 0:
          b[pos] = float(0);
          break;

        case 1:
          b[pos] = float(0.15);
          break;

        case 2:
          b[pos] = float(0.50);
          break;

        case 3:
          b[pos] = float(0.75);
          break;

        case 4:
          b[pos] = float(1);
          break;
          
        default:
          Serial.println("switch case error");
      }
    } else {                                    //for the rest half of total epochs
      b[pos] = float(esp_random() % 15) / 100;  //small mutations
    }


  } while (equivalentArray(a, b) || equivalentArray(b, temp));

  //////////////////////////////////////////////////
  Serial.println(" after " + (String)b[pos]);
}
bool equivalentArray(float* a, float* b) {
  for (int k = 0; k < len; k++) {
    if (a[k] != b[k]) return false;
  }
  return true;
}


void startWIFI() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address on serial monitor
  Serial.print("My IP is: ");
  Serial.print(WiFi.localIP());
}

void clearBuffer(char* buffer) {
  int len = strlen(buffer);
  for (int i = 0; i < len; i++) {
    buffer[i] = 0;
  }
}
