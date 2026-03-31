#include <esp_now.h>
#include <WiFi.h>
#include <AccelStepper.h>

#define STEP_PIN_X 16
#define DIR_PIN_X 4
#define STEP_PIN_Y 13
#define DIR_PIN_Y 15
const int dis=200;
const int wait_time=15000;
AccelStepper stepperX(AccelStepper::DRIVER, STEP_PIN_X, DIR_PIN_X);
AccelStepper stepperY(AccelStepper::DRIVER, STEP_PIN_Y, DIR_PIN_Y);
// Message structure
struct Message {
  int distance;
};
long yPosition=0;
long ytp=0;//y target position
unsigned long lmt=0;//last move time
Message receivedMsg;
bool msg = false;
//const esp_now_recv_info -> structure defined in esp-now library , it contains mac address  
void onReceive(const esp_now_recv_info* recv_info, const uint8_t* incomingData, int len)
{
  memcpy(&receivedMsg, incomingData, sizeof(receivedMsg));
  // Serial.print("Alert received! Distance: ");
  // Serial.println(receivedMsg.distance);
  if(receivedMsg.distance<10)
    msg = true;
}  
void setup() {
  Serial.begin(115200);
  // Set ESP32 as a Wi-Fi station like a phone 
  WiFi.mode(WIFI_STA);
  Serial.println("ESP-NOW Slave Initialized");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");


    
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(onReceive);
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(800);  
  stepperY.setAcceleration(400);
  // stepperY.setSpeed(200);   
}

void loop() {
  if (!msg) 
  {
    if(millis()-lmt>=wait_time)
    {
      ytp-=dis;//set the position to ytp to move the stepper motor by that many steps;
      // 200 steps=15cm
      stepperY.moveTo(ytp);
      while (stepperY.distanceToGo() != 0) 
      { 
        stepperY.run();
      }
      lmt=millis();
    }  
  }
   if (msg) {

    stepperX.moveTo(500);  // 
    stepperX.runToPosition();  // Block until Stepper X reaches the target position

    // Now move Stepper Y up to its starting position
    while (yPosition < 0) {  // While Y is below start position
      stepperY.setSpeed(200);  // set the speed to move
      stepperY.runSpeed();  // move stepper as the speed is mentioned
      yPosition++;   // Increase position as it moves up
    }
    msg=false;
    ytp=0;
   }
   delay(10);
}

