#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define WIFI_SSID "vivo12"
#define WIFI_PASSWORD "123456789"

String sheet_url = "https://script.google.com/macros/s/AKfycbwW35TbC9obL8GtAoUn20nGt7ItYo2hZchhZe6PNnWR4YgrmQGtxgMmJBhfCZGSu-47/exec";

// Buttons
const int remove_button = 32;
const int add_button = 33;
const int reset_button = 25;
const int bill_button = 13;

// LEDs + BUZZER
const int greenLED = 12;
const int redLED = 14;
const int blueLED = 27;
const int buzzer_Pin = 26;

// RFID
#define SS_PIN 5
#define RST_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ITEMS
struct item {
  String name;
  String uid;
  int price;
};

item items[] = {
  {"Tata salt","D3 A0 91 FB",100},
  {"Milk Powder","29 84 5E B7",50},
  {"Bournvita","49 17 AE B7",20},
  {"Ball Pen","53 80 59 F4",10},
  {"Pakoda","29 0B 96 B7",1000},
  {"Chutney","19 E9 E0 B7",10}
};

int totalItems = 6;
int bill_amount = 0;
bool addMode = true;

// 🔥 DISPLAY CONTROL
bool showItem = false;
unsigned long displayTime = 0;

// BUZZER
void beep() {
  digitalWrite(buzzer_Pin, HIGH);
  delay(150);
  digitalWrite(buzzer_Pin, LOW);
}

// URL encode
String urlencode(String str) {
  String encoded = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ' ') encoded += "%20";
    else encoded += str[i];
  }
  return encoded;
}

// WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

// SEND
void sendToSheet(String name, int price, String mode) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = sheet_url +
               "?name=" + urlencode(name) +
               "&price=" + String(price) +
               "&mode=" + mode;

  http.begin(client, url);
  http.GET();
  http.end();
}

// RESET
void resetSheet() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, sheet_url + "?reset=true");
  http.GET();
  http.end();
}

// BILL
void sendBill() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, sheet_url + "?bill=true");
  http.GET();
  http.end();
}

void setup() {

  Serial.begin(115200);

  pinMode(remove_button, INPUT_PULLUP);
  pinMode(add_button, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);
  pinMode(bill_button, INPUT_PULLUP);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buzzer_Pin, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.print("Smart Trolley");
  delay(2000);

  connectWiFi();
}

void loop() {

  if (digitalRead(add_button) == LOW) {
    addMode = true;
    lcd.clear();
    lcd.print("ADD MODE");
    delay(800);
  }

  if (digitalRead(remove_button) == LOW) {
    addMode = false;
    lcd.clear();
    lcd.print("REMOVE MODE");
    delay(800);
  }

  if (digitalRead(reset_button) == LOW) {
    bill_amount = 0;
    resetSheet();
    lcd.clear();
    lcd.print("RESET DONE");
    delay(1500);
  }

  if (digitalRead(bill_button) == LOW) {
    sendBill();
    lcd.clear();
    lcd.print("BILL SENT");
    digitalWrite(blueLED, HIGH);
    delay(500);
    digitalWrite(blueLED, LOW);
    delay(2000);
  }

  digitalWrite(greenLED, addMode);
  digitalWrite(redLED, !addMode);

  // RFID SCAN
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String uid = "";

    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    uid.toUpperCase();

    for (int i = 0; i < totalItems; i++) {

      if (uid.substring(1) == items[i].uid) {

        beep();

        lcd.clear();

        if (addMode) {
          bill_amount += items[i].price;

          lcd.print(items[i].name);
          lcd.setCursor(0, 1);
          lcd.print("+");
          lcd.print(items[i].price);

        } else {

          bill_amount -= items[i].price;
          if (bill_amount < 0) bill_amount = 0;

          lcd.print(items[i].name);
          lcd.setCursor(0, 1);
          lcd.print("Removed");
        }

        sendToSheet(items[i].name, items[i].price, addMode ? "add" : "remove");

        showItem = true;
        displayTime = millis();

        delay(1500);
        break;
      }
    }

    mfrc522.PICC_HaltA();
  }

  // ✅ SMART DISPLAY
  if (showItem) {

    if (millis() - displayTime > 3000) {
      showItem = false;
      lcd.clear();
    }

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Total:");
    lcd.setCursor(0, 1);
    lcd.print(bill_amount);
    lcd.print(" Rs");

  }

  delay(200);
}
