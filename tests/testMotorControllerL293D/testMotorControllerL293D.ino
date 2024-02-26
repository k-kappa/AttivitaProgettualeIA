/*#include <WiFi.h>
 
const char *ssid = "Vodafone-A40200668";
const char *password = "6Y9J7PRhLhAt76HP";

uint8_t Pwm1 = 12; //Nodemcu PWM pin 
//uint8_t Pwm2 = D2; //Nodemcu PWM pin

//Seven segment pins attachecd with nodemcu pins  
int a0 = 14;  //Gpio-15 of nodemcu esp8266  
int a1 = 27;  //Gpio-13 of nodemcu esp8266    
//int a2 = 12;  //Gpio-12 of nodemcu esp8266   
//int a3 = 14;  //Gpio-14 of nodemcu esp8266   


WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
//Declaring l293d control pins as Output
pinMode(a0, OUTPUT);     
pinMode(a1, OUTPUT);     
//pinMode(a2, OUTPUT);
//pinMode(a3, OUTPUT);    
 
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  digitalWrite(a0, HIGH); //Start first motor
  digitalWrite(a1, LOW);
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 int Ms=0,dir=0,Pw=0;
 
 // Match the request
 if (request.indexOf("/start=1") != -1)  {  
  digitalWrite(a0, HIGH); //Start first motor
  digitalWrite(a1, LOW);

  //digitalWrite(a2, HIGH); //Start second motor
  //digitalWrite(a3, LOW);
  Ms=1;
  dir=1;
}

if (request.indexOf("/stop=1") != -1)  {  
  digitalWrite(a0, LOW); //Stop first motor
  digitalWrite(a1, LOW);

  //digitalWrite(a2, LOW); //Stop second motor
  //digitalWrite(a3, LOW);
  Ms=0;
}

if (request.indexOf("/tog=1") != -1)  {
  digitalWrite(a0, LOW);  //Change First motor rotation direction
  delay(5000); //5 seconds delay
  digitalWrite(a1, HIGH);
  
  //digitalWrite(a2, LOW); //Change Second motor rotation direction
  //delay(5000); //5 seconds delay
  //digitalWrite(a3, HIGH);
  dir=0;
}

if (request.indexOf("/Req=2") != -1)  {  
analogWrite(Pwm1, 767);  //Pwm duty cycle 75%
//analogWrite(Pwm2, 767);  //Pwm duty cycle 75%
Pw=1;
}
if (request.indexOf("/Req=3") != -1)  { 
analogWrite(Pwm1, 512);  //Pwm duty cycle 50%
//analogWrite(Pwm2, 512);  //Pwm duty cycle 50%
Pw=2;
}
if (request.indexOf("/Req=4") != -1)  {  
analogWrite(Pwm1, 255);  //Pwm duty cycle 25%
//analogWrite(Pwm2, 255);  //Pwm duty cycle 25%
Pw=3;
}

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>Nodemcu Dc motor control over WiFi</h1><br><br>");
  client.println("<br><br>");
  client.println("<a href=\"/start=1\"\"><button>Start Motor </button></a><br/>");
  client.println("<a href=\"/stop=1\"\"><button>Stop Motor </button></a><br/>");
  client.println("<a href=\"/tog=1\"\"><button>Toggle Direction</button></a><br/>");
  client.println("<a href=\"/Req=2\"\"><button>Duty cycle 75% </button></a><br/>");
  client.println("<a href=\"/Req=3\"\"><button>Duty cycle 50% </button></a><br/>");
  client.println("<a href=\"/Req=4\"\"><button>Duty cycle 25% </button></a><br/>");

  if(Ms==1){
    client.println("Motor Powered Working<br/>" );
    }
    else
    client.println("Motor at Halt<br/>" );

  if(dir==1){
    client.println("Motor rotating in forward direction<br/>" );
    }
    else
    client.println("Motor rotating in backward direction<br/>" );

switch(Pw){
      case 1:
        client.println("Pwm duty cycle 75%<br/>" );
        break;
      case 2:
        client.println("Pwm duty cycle 50%<br/>" );
        break;  
      case 3:
        client.println("Pwm duty cycle 25%<br/>" );
        break; 
         
      default:
        client.println("Pwm duty cycle 100%<br/>" );
  }
  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}*/

#include <MotorL293DControllerK.h>
//MotorL293DControllerK motor1(25,33,26,8,30000);
//MotorL293DControllerK motor2(27,14,12,8,30000);
#define MOTOR1FOWARDPIN 0//25
#define MOTOR1BACKWARDPIN 2//33
#define MOTOR1ENABLEPIN 5//26

/*int motor1Pin1 = 33; //14  //25
int motor1Pin2 = 25; //27  //33
int enable1Pin = 26; //12  //26
 */
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;
 
void setup() {
  // sets the pins as outputs:
  pinMode(MOTOR1FOWARDPIN, OUTPUT);
  pinMode(MOTOR1BACKWARDPIN, OUTPUT);
  pinMode(MOTOR1ENABLEPIN, OUTPUT);
  //motor1.begin();
  //motor2.begin();
  
  // configure LED PWM functionalitites
  //ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(enable1Pin, pwmChannel);
 
  Serial.begin(9600);
 
  // testing
  Serial.print("Testing DC Motor...");
}
 
void loop() {
  /*motor1.moveForward();
  motor2.moveForward();
  delay(3000);
  motor1.moveBackward();
  motor2.moveBackward();
  delay(3000);
  Serial.println(motor1.setPWM(200));
  Serial.println(motor2.setPWM(200));
  delay(3000);
  motor1.stop();
  motor2.stop();
  delay(3000);
  Serial.println(motor1.setPWM(255));
  Serial.println(motor2.setPWM(255));*/
  /*ledcWrite(pwmChannel, 255);
  // Move the DC motor forward at maximum speed
  */Serial.println("Moving Forward");
  digitalWrite(MOTOR1FOWARDPIN, LOW);
  digitalWrite(MOTOR1BACKWARDPIN, HIGH); 
  analogWrite(MOTOR1ENABLEPIN, 250);
  delay(2000);
 /*
  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);
 
  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  delay(2000);
 
  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);
 
  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  */
}