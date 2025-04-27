#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Constants
const byte address[6] = "SECURE";  // Better address format
const unsigned int MAX_MESSAGE_LENGTH = 32;
const String ENCRYPTION_KEYS = "ArduSecure2023!"; // Stronger key

RF24 radio(9, 10); // CE, CSN pins

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial connection
  
  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1); // Stop if radio fails
  }
  
  radio.setPALevel(RF24_PA_MAX); // Max power for better range
  radio.setDataRate(RF24_250KBPS); // Lower speed for more reliability
  radio.openWritingPipe(address);
  
  Serial.println("Secure Transmitter Ready");
  Serial.println("Enter messages to send (max 30 chars):");
}

String encryptMessage(String message) {
  String encrypted = message;
  for (int i = 0; i < encrypted.length(); i++) {
    // XOR with rotating key for better security
    char key = ENCRYPTION_KEYS[i % ENCRYPTION_KEYS.length()];
    encrypted[i] = encrypted[i] ^ key;
    
    // Additional simple shift cipher
    encrypted[i] = encrypted[i] + 3;
  }
  return encrypted;
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    message.trim(); // Remove any extra whitespace
    
    if (message.length() > MAX_MESSAGE_LENGTH - 2) {
      Serial.println("Error: Message too long!");
      return;
    }
    
    String encrypted = encryptMessage(message);
    
    // Add checksum
    byte checksum = 0;
    for (int i = 0; i < encrypted.length(); i++) {
      checksum ^= encrypted[i];
    }
    encrypted += (char)checksum;
    
    // Send message
    bool success = radio.write(encrypted.c_str(), encrypted.length() + 1);
    
    if (success) {
      Serial.print("Sent: '");
      Serial.print(message);
      Serial.print("' as encrypted: ");
      Serial.println(encrypted);
    } else {
      Serial.println("Transmission failed!");
    }
  }
}