#ifndef CONFIG_H
#define CONFIG_H

// WiFi Credentials
const char* ssid = "admin";
const char* password = "12345678";

// Green API details
const char* apiUrl = "https://7103.api.greenapi.com/waInstance7103165247/sendMessage/d714efc2a8d34d9f96bc5c28d5d8f5fa5d93cba1d847454d93";
const char* phoneNumber = "919797904820@c.us";  // Recipient's WhatsApp number

// ThingSpeak API details
const char* thingSpeakApiKey = "YOUR_THINGSPEAK_API_KEY";
const char* thingSpeakServer = "api.thingspeak.com";

// Flow sensor pins
#define FLOW_SENSOR1_PIN 23
#define FLOW_SENSOR2_PIN 32

// Water usage thresholds (in Liters)
const float threshold1 = 10.0;
const float threshold2 = 15.0;
const float threshold3 = 20.0;
const float finePerLiter = 1.0;  // Fine per extra liter

#endif
