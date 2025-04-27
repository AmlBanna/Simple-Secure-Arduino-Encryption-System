# 📡 Wireless Encrypted Messaging using NRF24L01 & Arduino


## 📝 Description
A secure wireless communication system featuring:
- **NRF24L01** for 2.4GHz wireless transmission  
- **I2C LCD** for real-time message display  
- **Hybrid Encryption** (XOR + Shift Cipher)  
- **Checksum verification** for data integrity  

**Workflow**:
1. Message entered via Serial Monitor (Transmitter)
2. Encryption using `XOR + Shift (+3)`
3. Wireless transmission
4. Decryption & display on Receiver's LCD + Serial Monitor

---

## 📦 Hardware Components
| Component               | Qty | Notes                     |
|-------------------------|-----|---------------------------|
| Arduino Uno/Nano        | 2   |                           |
| NRF24L01 Module         | 2   | 3.3V only!                |
| I2C LCD (16x2)          | 1   | Address 0x27/0x3F         |
| 10KΩ Resistor           | 2   | NRF stability             |
| Jumper Wires            | -   |                           |
| Power Source            | 2   | USB/Battery               |

---

## 🔌 Circuit Diagram

### 1️⃣ Transmitter Wiring
| Arduino Pin | NRF24L01 Pin |
|-------------|--------------|
| 3.3V        | VCC          |
| GND         | GND          |
| D9          | CE           |
| D10         | CSN          |
| D11 (MOSI)  | MOSI         |
| D12 (MISO)  | MISO         |
| D13 (SCK)   | SCK          |

## ⚠️ Important:

Use only 3.3V for NRF24L01 (not 5V).

Add 10KΩ resistor between VCC and GND for signal stability.


### 2️⃣ Receiver Wiring
| Arduino Pin | NRF24L01 Pin | I2C LCD Pin |
|-------------|--------------|-------------|
| 3.3V        | VCC          | VCC         |
| GND         | GND          | GND         |
| D9          | CE           | -           |
| D10         | CSN          | -           |
| D11         | MOSI         | -           |
| D12         | MISO         | -           |
| D13         | SCK          | -           |
| A4 (SDA)    | -            | SDA         |
| A5 (SCL)    | -            | SCL         |

⚠️ **Critical Notes**:
- Power NRF24L01 **only with 3.3V**
- Add 10KΩ resistor between NRF's VCC-GND
- LCD I2C address typically `0x27` or `0x3F`

---
## ⚙️ How It Works

### Hardware Setup:
1. Connect components as per wiring diagrams
2. Power both devices independently (3.3V for NRF modules)

### Software Setup:
```bash
# Required Libraries
- RF24.h            # NRF24L01 control
- LiquidCrystal_I2C # LCD interface
```
### Operation Flow:

Open Serial Monitor (Transmitter @ 9600 baud)

Enter plaintext message

System will:

Encrypt using XOR+Shift cipher

Transmit via NRF24L01

Receive and decrypt on other end

Display on LCD + Serial Monitor
## 🛠️ Troubleshooting

| Issue                      | Solution                                                                 |
|----------------------------|--------------------------------------------------------------------------|
| **NRF24L01 not responding** | 1. Verify 3.3V power supply<br>2. Check 10KΩ resistors between VCC-GND<br>3. Ensure proper SPI pin connections |
| **LCD not displaying**      | 1. Confirm I2C address (try `0x27` or `0x3F`)<br>2. Check SDA/SCL connections<br>3. Adjust LCD contrast potentiometer |
| **Corrupted messages**      | 1. Verify matching encryption keys on both devices<br>2. Check RF channel consistency<br>3. Reduce transmission distance |
| **Frequent disconnections** | 1. Add decoupling capacitors (10-100μF)<br>2. Avoid 2.4GHz interference (WiFi/bluetooth)<br>3. Check antenna orientation |
| **Arduino freezing**        | 1. Confirm adequate power supply<br>2. Check for serial port conflicts<br>3. Update RF24 library to latest version |

💡 **Pro Tips**:
- Use `radio.printDetails()` to debug NRF24L01 configuration
- Test with simple strings before full encryption
- Keep devices within 10m range for stable connection


## 🔐 Encryption Scheme

### Hybrid XOR + Shift Cipher
```cpp
/**
 * Encrypts message using XOR + Shift cipher
 * @param msg Plaintext message (String)
 * @return Encrypted String
 */
String encrypt(String msg) {
  const String key = "ArduSecure2023!"; // 16-byte key
  String encrypted;
  
  for (int i = 0; i < msg.length(); i++) {
    // XOR with rotating key
    char c = msg[i] ^ key[i % key.length()]; 
    // Additional shift (+3)
    encrypted += (c + 3);  
  }
  return encrypted;
}
```
## Checksum Verification
```cpp
/**
 * Verifies message integrity using 8-bit checksum
 * @param msg Encrypted message
 * @return true if checksum matches
 */
bool verifyChecksum(String msg) {
  if(msg.length() < 1) return false;
  
  uint8_t sum = 0;
  for (int i = 0; i < msg.length()-1; i++) {
    sum += msg[i]; // Sum all bytes except last
  }
  return (sum % 256 == msg[msg.length()-1]);
}
```

### 🔍 Cryptography Details:

### Key Length: 128-bit (16 chars)

### Algorithm: XOR + Caesar Shift (+3)

### Checksum: 8-bit modular sum

### Security Level: Basic (educational purpose)

## 🚀 Future Improvements



| Feature               | Status      | Priority  | Estimated | Dependencies           |
|-----------------------|-------------|-----------|-----------|------------------------|
| Keypad Input          | Planned     | Medium    | Q2 2024   | Hardware procurement   |
| AES-128 Encryption    | In Progress | High 🔥   | Q1 2024   | Crypto library         |
| SD Card Logging       | Backlog     | Low       | -         | SPI interface          |
| Dynamic Key Exchange  | Researching | Critical ⚠️ | -         | Security analysis      |

