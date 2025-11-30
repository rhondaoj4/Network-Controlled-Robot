#include <WiFi.h>

// --- Configuration ---
const char* ssid     = "wifi_name"; 
const char* password = "password"; 

WiFiServer server(8080); 

// --- Pin Definitions ---
const int EN_A = 14; // Left Motor Enable Pin 
const int IN1  = 26; // Left Motor Input 1 (Direction)
const int IN2  = 27; // Left Motor Input 2 (Direction)

const int EN_B = 32; // Right Motor Enable Pin
const int IN3  = 25; // Right Motor Input 3 (Direction)
const int IN4  = 33; // Right Motor Input 4 (Direction)

const int LED_PIN = 2; 



void setMotor(int enPin, int in1Pin, int in2Pin, bool forward) {
  // 1. Set Direction
  if (forward) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  }
  // 2. Enable Motor: Turn it ON (HIGH = Full Speed)
  digitalWrite(enPin, HIGH); 
}

void goForward() {
  digitalWrite(LED_PIN, HIGH); // DEBUG: LED ON
  setMotor(EN_A, IN1, IN2, false); // Left Motor (Forward)
  setMotor(EN_B, IN3, IN4, true);  // Right Motor (Forward)
}

void goBackward() {
  digitalWrite(LED_PIN, HIGH); // DEBUG: LED ON
  setMotor(EN_A, IN1, IN2, true);  // Left Motor (Backward)
  setMotor(EN_B, IN3, IN4, false); // Right Motor (Backward)
}

void goLeft() {
  digitalWrite(LED_PIN, HIGH); // DEBUG: LED ON
  setMotor(EN_A, IN1, IN2, true);  // Left Motor (Backward for pivot)
  setMotor(EN_B, IN3, IN4, true);  // Right Motor (Forward for pivot)
}

void goRight() {
  digitalWrite(LED_PIN, HIGH); // DEBUG: LED ON
  setMotor(EN_A, IN1, IN2, false); // Left Motor (Forward for pivot)
  setMotor(EN_B, IN3, IN4, false); // Right Motor (Backward for pivot)
}

void allStop() {
  digitalWrite(LED_PIN, LOW); 
  // 1. Disable Power
  digitalWrite(EN_A, LOW); 
  digitalWrite(EN_B, LOW); 
  // 2. Set inputs LOW 
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}


// --- ARDUINO SETUP ---

void setup() {
  Serial.begin(115200);

  // Initialize all pins as outputs
  pinMode(LED_PIN, OUTPUT); 
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(EN_A, OUTPUT); pinMode(EN_B, OUTPUT);
  
  allStop(); 

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Flash LED while connecting
    delay(200);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 

  digitalWrite(LED_PIN, LOW); // LED OFF means connected
  server.begin();
}


// --- ARDUINO LOOP - TCP COMMAND PARSER ---

void loop() {
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client Connected.");
    
    while (client.connected()) {           
      if (client.available()) {             
        String command = client.readStringUntil('\n');
        command.trim(); 

        if (command.length() > 0) {
            Serial.print("Received: ");
            Serial.println(command);
            
            // --- Command Execution ---
            if (command == "FORWARD") {
                goForward();
            } else if (command == "BACKWARD") {
                goBackward();
            } else if (command == "LEFT") {
                goLeft();
            } else if (command == "RIGHT") {
                goRight();
            } else if (command == "STOP") {
                allStop();
            }
            delay(10); 
        }
      }
    }
    
    Serial.println("Client Disconnected.");
    allStop(); 
  }
}