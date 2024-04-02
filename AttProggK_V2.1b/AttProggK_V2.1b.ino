#include <Arduino.h>
#include <UltrasonicSensorK.h>
#include <MotorL293DControllerK.h>
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

const char *ssid = "";
const char *password = "";

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
void applyMutation(float* parameters, float* returnArray);

void clearBuffer(char* buffer);

//Global variables
int len = 4;


void inviaDati(char* dato) {
  Udp.beginPacket("192.168.1.5", 2025);
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

  digitalWrite(LED,HIGH);

  //connect wifi
  startWIFI();
}

void loop() {
  Serial.println("----------------------------");

  int totEpoch = 30;
  unsigned long secondsEpoch = 30;
  float bestScore=0;
  float parameters[len];
  float bestParameters[len];

  parameters[0]=float(esp_random()%2); //here we are considering booleans
  delay(10);
  parameters[1]=float(esp_random()%2);
  delay(10);
  parameters[2]=float(esp_random()%2);
  delay(10);
  parameters[3]=float(esp_random()%2);
  /*parameters[0]=0;
  parameters[1]=1;
  parameters[2]=0;
  parameters[3]=1;*/

  

  int pos = 0;

  char buffer[100];
  sprintf(buffer,"\n SESSION STARTED \n");
  inviaDati(buffer);
  clearBuffer(buffer);

  buffer[100];
  sprintf(buffer," >Starting epoch %d with configuration: a=%f b=%f c=%f d=%f",0,parameters[0],parameters[1],parameters[2],parameters[3]);
  inviaDati(buffer);
  clearBuffer(buffer);
  float score = startEpoch(0,secondsEpoch,parameters,90,130);
  //copy the best scores with relative parameters
  bestScore = score;
  //Serial.println("dim "+(String)(sizeof(parameters)/sizeof(parameters[0])));
  for(int i=0;i<len;i++){
    bestParameters[i] = parameters[i];
  }
  buffer[100];
  sprintf(buffer,"new best score %f with configuration: a=%f b=%f c=%f d=%f",score,parameters[0],parameters[1],parameters[2],parameters[3]);
  inviaDati(buffer);
  clearBuffer(buffer);
  //apply mutation
  applyMutation(bestParameters,parameters);
  for(int g=0;g<len;g++){
    Serial.println((String)bestParameters[g]+" = "+(String)parameters[g]);
  }
  
  for(int i=0;i<(totEpoch-1);i++){

    char buffer[100];
    sprintf(buffer,"\n >Starting epoch %d with configuration: a=%f b=%f c=%f d=%f",i+1,parameters[0],parameters[1],parameters[2],parameters[3]);
    inviaDati(buffer);
    clearBuffer(buffer);
    //start epoch
    Serial.println("starting epoch "+(String)(i+1));
    float score = startEpoch(i,secondsEpoch,parameters,50,130);
    //check the score and send data to PC
    if(score<bestScore){  //new best score found
      char buffer[100];
      sprintf(buffer,"new best score %f with configuration: a=%f b=%f c=%f d=%f ",score,parameters[0],parameters[1],parameters[2],parameters[3]);
      inviaDati(buffer);
      clearBuffer(buffer);
      bestScore = score;
      for(int h=0;h<len;h++){
        bestParameters[h] = parameters[h];
      }
    }else{ //just notifying the score
      char buffer[100];
      sprintf(buffer,"score %f with configuration: a=%f b=%f c=%f d=%f ",score,parameters[0],parameters[1],parameters[2],parameters[3]);
      inviaDati(buffer);
      clearBuffer(buffer);
    }
    //apply the mutation
    for(int g=0;g<4;g++){
      Serial.println("from "+(String)bestParameters[g]+" a mutation may be applied  ");
    }
    applyMutation(bestParameters,parameters);
    for(int g=0;g<len;g++){
      Serial.println("after "+(String)bestParameters[g]+" = "+(String)parameters[g]);
    }

    motor1.stop();
    motor2.stop();
    digitalWrite(LED,LOW);
    delay(500);
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW);
    delay(500);
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW);
    delay(500);
    digitalWrite(LED,HIGH);
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

float startEpoch(int numEpoch, unsigned long secondsEpoch, float* parameters, float amplifier, int v0){
  unsigned long startTime = millis();
  float score = 0;
  unsigned long fx1,fx2;
  int m1,m2;
  bool direction = 0; //specify the direction of the robot(0=forward,1=backward)
  int collisionCounter=0;
  int collisionAdder = 50;
  while(millis()-startTime<=secondsEpoch*1000){
    if(!direction){//Forward
       //measure distances
      float d1 = UltraS1.measureDistance();
      delay(30);
      float d3 = UltraS3.measureDistance();
      delay(30);
      int flag, wall;
      if(d1 < 8 || d3 <8) flag = 0;
      else flag = 1;
      if(abs(d1-d3)<4) wall=1;
      else wall=0;
      //apply the functions to identify the speed motor
      fx1 = (v0 + sensorFunction(d1,amplifier)*parameters[0] + sensorFunction(d3,amplifier)*parameters[3]);
      fx2 = (v0 + sensorFunction(d1,amplifier)*parameters[1] + sensorFunction(d3,amplifier)*parameters[2]);
      //collect the score
      score = score + 1/d1 + 1/d3;
      if(wall==0 && flag==0){
        score = score + collisionAdder; //in case of collision worsens the score
      }
      //set range constraint
      m1 = constrain(fx1, MINMOTORVALUE, MAXMOTORVALUE);
      m2 = constrain(fx2, MINMOTORVALUE, MAXMOTORVALUE);
      //set the motor speed
      motor1.setPWM(m1);
      motor2.setPWM(m2);
      if (flag == 1){
        motor1.moveForward();
        motor2.moveForward();
      }else{
        motor1.stop();
        motor2.stop();
        direction = 1;
        collisionCounter++;
        delay(1000);
      }
    }else{//Backward

      //offset sensor during backward movemnets
      int offset_distance = 0; //cm
      //measure distances
      float d4 = UltraS4.measureDistance()+offset_distance;
      delay(30);
      float d6 = UltraS6.measureDistance()+offset_distance;
      delay(30);
      int flag,wall;
      if(d4 < 8 || d6 <8) flag = 0;
      else flag = 1;
      if(abs(d4-d6)<4) wall=1;
      else wall=0;
      //apply the functions to identify the speed motor
      fx1 = (v0 + sensorFunction(d4,amplifier)*parameters[2] + sensorFunction(d6,amplifier)*parameters[1]);
      fx2 = (v0 + sensorFunction(d4,amplifier)*parameters[3] + sensorFunction(d6,amplifier)*parameters[0]);
      //collect the score
      score = score + 1/d4 + 1/d6;
      if(wall==0 && flag==0){
        score = score + collisionAdder; //in case of collision worsens the score
      }
      //set range constraint
      m1 = constrain(fx1, MINMOTORVALUE, MAXMOTORVALUE);
      m2 = constrain(fx2, MINMOTORVALUE, MAXMOTORVALUE);
      //set the motor speed
      motor1.setPWM(m1);
      motor2.setPWM(m2);
      if (flag == 1){
        motor1.moveBackward();
        motor2.moveBackward();
      }else{
        motor1.stop();
        motor2.stop();
        direction = 0;
        collisionCounter++;
        delay(1000);
      }
    }
  }

  char buffer[100];
  sprintf(buffer,"collision counter: %d",collisionCounter);
  inviaDati(buffer);
  clearBuffer(buffer);

  return score;
}

float sensorFunction(float distance, float amplifier){
  return pow(50/(distance+1),2)*amplifier;  
}

void applyMutation(float* a,float* b){
  Serial.println("dim interna "+(String)len);
  for(int j=0;j<len;j++){
    b[j] = a[j];
  }
  long pos = esp_random()%len;
  Serial.println("mutation applied at pos "+(String)pos);
  Serial.print("before "+(String)a[pos]);
  //b[pos] = float(esp_random()%2); // boolean
  //the following instruction just for boolean world
  if(b[pos]==0) b[pos]=1;
  else b[pos]=0;
  //////////////////////////////////////////////////
  Serial.println(" after "+ (String)b[pos]);
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

void clearBuffer(char* buffer){
  int len = strlen(buffer);
  for(int i=0;i<len;i++){
    buffer[i] = 0;
  }
}