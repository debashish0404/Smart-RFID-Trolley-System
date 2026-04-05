# RFID Smart Trolley System (ESP32 + WhatsApp Integration)

## Overview
This project is an IoT-based smart shopping trolley system using ESP32 and RFID technology. It automatically scans items, calculates the total bill in real-time, and sends the bill directly to the user via WhatsApp using Twilio API.

## Features
- RFID-based automatic item detection  
- Real-time billing system  
- Add / Remove item functionality  
- WiFi-based communication  
- WhatsApp notification using Twilio API  
- Contactless and efficient shopping  

## Tech Stack
- Hardware: ESP32, RFID RC522 Module  
- Software: Arduino IDE  
- Communication: WiFi (HTTP)  
- API: Twilio WhatsApp API  

## Hardware Components
- ESP32 Dev Board  
- RFID RC522 Reader  
- RFID Tags/Cards  
- Jumper Wires  
- Breadboard  

## Circuit Connections

RFID Module → ESP32  
VCC → 3.3V  
GND → GND  
SDA (SS) → GPIO 5  
SCK → GPIO 18  
MOSI → GPIO 23  
MISO → GPIO 19  
RST → GPIO 13  

## System Architecture

RFID Tag → ESP32 → HTTP Request → Twilio API → WhatsApp Message

## How It Works

1. RFID tag is scanned using RC522 module  
2. ESP32 reads UID and matches item  
3. Item price is added or removed from total  
4. ESP32 creates bill message  
5. HTTP POST request sent to Twilio API  
6. User receives bill instantly on WhatsApp  

## Setup Instructions

### 1. Upload Code to ESP32
- Open Arduino IDE  
- Install libraries:
  - MFRC522  
  - WiFi  
  - HTTPClient  
- Upload your code  

---

### 2. Setup Twilio WhatsApp

1. Create account on Twilio  
2. Activate WhatsApp Sandbox  
3. Get credentials:
   - Account SID  
   - Auth Token  
   - Twilio WhatsApp Number  

4. Join sandbox from your phone using given code  

---

### 3. Twilio API Endpoint

Use this URL:

https://api.twilio.com/2010-04-01/Accounts/YOUR_ACCOUNT_SID/Messages.json

---

### 4. ESP32 Code Configuration

Update:
- WiFi SSID & Password  
- Twilio Account SID  
- Auth Token  
- Phone numbers  

---

### 5. Example Message Format

Your Smart Trolley Bill:
Milk Powder - ₹50  
Bournvita - ₹20  
----------------------  
Total: ₹70  

---

### 6. Run the System

- Power ESP32  
- Open Serial Monitor (115200 baud)  
- Scan RFID tag  
- Bill updates automatically  
- WhatsApp message received instantly  

---

## Example Serial Output

Scan your RFID tag  
UID detected  
Item: Milk Powder  
Price: 50  
Total: 150  
Message sent to WhatsApp  

---

## Project Structure

RFID-Smart-Trolley-ESP32/
│── code/
│   └── smart_trolley.ino  
│── images/
│   ├── circuit.jpg  
│   ├── setup.jpg  
│── README.md  

---

## Future Improvements
- LCD display for bill  
- Mobile app integration  
- QR code payment system  
- Cloud database storage  
- Multi-user trolley system  

---

## Conclusion
This project demonstrates a smart IoT-based billing system that improves shopping efficiency by automating item detection and providing real-time WhatsApp notifications.

---

## Author
Your Name
