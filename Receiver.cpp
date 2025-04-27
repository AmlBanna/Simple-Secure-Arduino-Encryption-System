#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h> // For LCD display

// LCD setup (0x27 is common I2C address)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Constants
const byte address[6] = "SECURE";
const String ENCRYPTION_KEYS = "ArduSecure2023!";
const unsigned int MAX_MESSAGE_LENGTH = 32;

RF24 radio(9, 10); // CE, CSN pins

String decryptMessage(String encrypted) {
  String decrypted = encrypted;
  
  // Remove checksum first
  if (encrypted.length() < 1) return "";
  byte receivedChecksum = encrypted[encrypted.length()-1];
  decrypted = encrypted.substring(0, encrypted.length()-1);
  
  // Verify checksum
  byte calculatedChecksum = 0;
  for (int i = 0; i < decrypted.length(); i++) {
    calculatedChecksum ^= decrypted[i];
  }
  
  if (receivedChecksum != calculatedChecksum) {
    return "!CORRUPTED!";
  }
  
  // Decrypt the message
  for (int i = 0; i < decrypted.length(); i++) {
    // Reverse shift cipher
    decrypted[i] = decrypted[i] - 3;
    
    // XOR with rotating key
    char key = ENCRYPTION_KEYS[i % ENCRYPTION_KEYS.length()];
    decrypted[i] = decrypted[i] ^ key;
  }
  
  return decrypted;
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial connection
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Secure Receiver");
  
  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    lcd.setCursor(0, 1);
    lcd.print("Radio Error!");
    while (1);
  }
  
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, address);
  radio.startListening();
  
  Serial.println("Secure Receiver Ready");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
}

void loop() {
  if (radio.available()) {
    char receivedData[MAX_MESSAGE_LENGTH] = "";
    radio.read(&receivedData, sizeof(receivedData));
    
    String encrypted = String(receivedData);
    String decrypted = decryptMessage(encrypted);
    
    // Display on Serial
    Serial.print("Received: ");
    Serial.println(decrypted);
    
    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received:");
    lcd.setCursor(0, 1);
    
    if (decrypted == "!CORRUPTED!") {
      lcd.print("BAD TRANSMISSION");
      Serial.println("Error: Message corrupted!");
    } else {
      lcd.print(decrypted.substring(0, 16)); // Fit to LCD width
    }
  }
}