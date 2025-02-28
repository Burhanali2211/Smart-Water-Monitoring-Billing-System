#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ThingSpeak.h> // ThingSpeak library

// WiFi Credentials
const char* ssid = "admin";
const char* password = "12345678";

// Green API details
String apiUrl = "https://7103.api.greenapi.com/waInstance7103165247/sendMessage/d714efc2a8d34d9f96bc5c28d5d8f5fa5d93cba1d847454d93";
String phoneNumber = "919797904820@c.us";  // Recipient's WhatsApp number

// ThingSpeak details
unsigned long channelID = 123456; // Replace with your ThingSpeak channel ID
const char* apiKey = "YOUR_API_KEY_HERE"; // Replace with your ThingSpeak Write API Key
WiFiClient client; // ThingSpeak client

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Water flow sensor pins
#define FLOW_SENSOR1_PIN 23
#define FLOW_SENSOR2_PIN 32

// Thresholds and fine
const float threshold1 = 0.1;  // 10 Liters
const float threshold2 = 0.15;  // 15 Liters
const float threshold3 = 0.50;  // 20 Liters
const float finePerLiter = 1.0;  // 1 Dollar per Liter

// Flow sensor variables
volatile int pulseCount1 = 0;
volatile int pulseCount2 = 0;
float flowRate1 = 0.0;
float flowRate2 = 0.0;
float totalWater1 = 0.0;
float totalWater2 = 0.0;
bool messageSent1_10 = false;
bool messageSent1_15 = false;
bool messageSent1_20 = false;
bool messageSent2_10 = false;
bool messageSent2_15 = false;
bool messageSent2_20 = false;

// Flow sensor interrupts
void IRAM_ATTR flowSensor1_ISR() {
  pulseCount1++;
}

void IRAM_ATTR flowSensor2_ISR() {
  pulseCount2++;
}

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Welcome message
  display.println(F("__Welcome to Water__"));
  display.println(F("..Billing System.."));
  display.println(F("Made by Burhan Ali"));
  display.display();
  delay(3000);

  // Display Wi-Fi connecting status
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Connecting to Wi-Fi:"));
  display.println(F("SSID:"));
  display.println(ssid);
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    display.setCursor(0, 32);
    display.print(F("Status: Connecting..."));
    display.display();
  }

  Serial.println("Connected to WiFi");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Connected to Wi-Fi"));
  display.println(F("SSID:"));
  display.println(ssid);
  display.display();
  delay(2000);

  // Set up flow sensor pins
  pinMode(FLOW_SENSOR1_PIN, INPUT_PULLUP);
  pinMode(FLOW_SENSOR2_PIN, INPUT_PULLUP);

  // Attach interrupts for flow sensors
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR1_PIN), flowSensor1_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR2_PIN), flowSensor2_ISR, FALLING);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Calculate flow rates (Liters per minute)
  flowRate1 = pulseCount1 / 7.5;  // Example conversion factor
  flowRate2 = pulseCount2 / 7.5;  // Example conversion factor

  // Update total water usage (Liters)
  totalWater1 += flowRate1 / 60.0;  // Flow rate in Liters per second, accumulate to total liters
  totalWater2 += flowRate2 / 60.0;  // Flow rate in Liters per second, accumulate to total liters

  // Reset pulse counts for the next cycle
  pulseCount1 = 0;
  pulseCount2 = 0;

  // Log water usage to Serial Monitor
  Serial.print("House 1 Water Usage: ");
  Serial.print(totalWater1);
  Serial.println(" Liters");
  Serial.print("House 2 Water Usage: ");
  Serial.print(totalWater2);
  Serial.println(" Liters");

  // Display water usage on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("House 1: ");
  display.print(totalWater1, 1);
  display.println(" Liters");
  display.print("House 2: ");
  display.print(totalWater2, 1);
  display.println(" Liters");

  // Update ThingSpeak fields
  ThingSpeak.setField(1, totalWater1);
  ThingSpeak.setField(2, totalWater2);
  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("ThingSpeak update successful");
  } else {
    Serial.print("Error updating ThingSpeak: ");
    Serial.println(response);
  }

  // Check for threshold breach and fine conditions
  if (totalWater1 > threshold1 && totalWater1 <= threshold2 && !messageSent1_10) {
    sendMessage(1, totalWater1, threshold1);
    messageSent1_10 = true;
  } else if (totalWater1 > threshold2 && totalWater1 <= threshold3 && !messageSent1_15) {
    sendMessage(1, totalWater1, threshold2);
    messageSent1_15 = true;
  } else if (totalWater1 > threshold3 && !messageSent1_20) {
    sendMessage(1, totalWater1, threshold3);
    messageSent1_20 = true;
  }

  if (totalWater2 > threshold1 && totalWater2 <= threshold2 && !messageSent2_10) {
    sendMessage(2, totalWater2, threshold1);
    messageSent2_10 = true;
  } else if (totalWater2 > threshold2 && totalWater2 <= threshold3 && !messageSent2_15) {
    sendMessage(2, totalWater2, threshold2);
    messageSent2_15 = true;
  } else if (totalWater2 > threshold3 && !messageSent2_20) {
    sendMessage(2, totalWater2, threshold3);
    messageSent2_20 = true;
  }

  // Display fine information on OLED
  if (totalWater1 > threshold1 || totalWater2 > threshold1) {
    display.setCursor(0, 32);
    display.print(" Water Limit Exceeded.");
  }

  display.display();
  delay(2000);  // Delay to avoid flooding the OLED
}

void sendMessage(int house, float totalWater, float threshold) {
  float extraUsage = totalWater - threshold;
  float fineAmount = extraUsage * finePerLiter;
  String message = "House " + String(house) + ": *Water usage exceeded " + String(threshold) + "L*\nExtra: " + String(extraUsage) + "L\nFine: " + String(fineAmount) + " USD.";

  Serial.println("Sending WhatsApp message: " + message);

  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");

  // Create the JSON payload
  StaticJsonDocument<200> doc;
  doc["chatId"] = phoneNumber;
  doc["message"] = message;

  String jsonData;
  serializeJson(doc, jsonData);

  // Send the POST request
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode == 200) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.print("Error sending message. Response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
