// Dichiarazione della costante per il numero del pin del LED
const int ledPin = 2;
static int taskCore = 0;
 
void coreTask( void * pvParameters ){
 
    String taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
 
    while(true){
        Serial.println(taskMessage);
        // Accensione del LED
        digitalWrite(ledPin, HIGH);
        delay(1000); // Attendi 1 secondo
  
        // Spegnimento del LED
          digitalWrite(ledPin, LOW);
        delay(1000); // Attendi 1 secondo
    }
 
}

void setup() {
  // Inizializzazione del pin del LED come output
  pinMode(ledPin, OUTPUT);
  Serial.begin(112500);
  delay(1000);
 
  Serial.print("Starting to create task on core ");
  Serial.println(taskCore);

  xTaskCreatePinnedToCore(
                    coreTask,   /* Function to implement the task */
                    "coreTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
 
  Serial.println("Task created...");
}

void loop() {
  Serial.println("Starting main loop...");
  while (true){}
  
}
