#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define IN1 25
#define IN2 26
#define ENA 33
#define TRIG_PIN 5   // Ultrasonic sensor TRIG pin
#define ECHO_PIN 18  // Ultrasonic sensor ECHO pin
#define THRESHOLD 10 // Distance threshold in cm

Servo roll;
Servo gate;
// MAC Address of the slave ESP32
uint8_t slaveAddress[] = {0xCC, 0xDB, 0xA7, 0x2F, 0x84, 0xCC};

// Message to send
struct Message {
  int distance;
};
Message msg;
// go to line 46 to know this.
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  Serial.begin(115200);
  gate.attach(21);
  roll.attach(20);
  roll.write(0); 
  gate.write(0);
 
  // Set ESP32 as a Wi-Fi station
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Not Initialized");
      return;
    }
  // This function registers call back function such that when a msg is sent to the other device irrespective of the placing of the function the onsent finction will activate*
  esp_now_register_send_cb(onSent);

  // Add peer (slave)
  esp_now_peer_info_t peerInfo;//It is a structure by esp now to store the information this structure holds the details about the peer device such as its mac address.
  memcpy(peerInfo.peer_addr, slaveAddress, 6); 

  peerInfo.channel = 0;//To keep the channel of the slave esp but since we are using the same wifi there is no need to use a specific channel
  peerInfo.encrypt = false;// There is no need to keep the msg encrypted so no issues.

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }//it is used to check whether a peer(slave) is connected or not.

  pinMode(TRIG_PIN, OUTPUT);  // Set TRIG_PIN as output
  pinMode(ECHO_PIN, INPUT);   // Set ECHO_PIN as input
}

void loop() {
  long duration, distance;
  // dipping();
  gate.write(90);
  delay(3000);
  gate.write(0);
  unsigned long st = millis();//start time
  unsigned long moveDur= 15000; // 15 sec
  bool forw = true; // flag for direction
  
  while (millis() - st < moveDur) {
    if (forw) {
      rpforward(); // Move forward
    } else {
      rpbackward(); // Move backward
    }
    
    // Alternate between forward and backward every 3 seconds(assuming it would take the rack and pinion 3 sec to move forward)
    if (millis() - st >= 3000) {
      forw = !forw;
      st = millis(); // Reset the timer to track next 3 seconds
    }
  }
  // delay(1000);
  // rpforward();
  // delay(3000);
  // rpbackward();
  // Trigger the ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in cm
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < THRESHOLD) {
    msg.distance = distance;

    // Send message to slave
    esp_now_send(slaveAddress, (uint8_t *)&msg, sizeof(msg));//It is used to send the msg structure to other slave.
    // Serial.println("Alert sent to slave!");
    rollback();
    // delay(5000);
  }
  else
  {
    roll.write(0);
  }

  delay(500); // Wait before the next measurement
}
void dipping()
{
  roll.write(0);
  delay(100);
  roll.write(10);
  delay(100);
  roll.write(0);
}
void rollback()
{
  roll.write(90);
  // delay(300000)
}
void rpbackward()
{

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,127);
  for (int angle = roll.read(); angle >= 0; angle--) { 
    roll.write(angle);
    delay(20); 
  }
 
}
void rpforward()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,127);
     for (int angle = roll.read(); angle <= 120; angle++) { 
    roll.write(angle);
    delay(20); 
  }
}