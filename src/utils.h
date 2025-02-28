#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

void sendMessage(int house, float totalWater, float threshold) {
    float extraUsage = totalWater - threshold;
    float fineAmount = extraUsage * 1.0; // Fine per liter
    String message = "House " + String(house) + ": *Water usage exceeded " + String(threshold) + "L*\nExtra: " + String(extraUsage) + "L\nFine: " + String(fineAmount) + " USD.";
    
    Serial.println("Sending WhatsApp message: " + message);

    HTTPClient http;
    http.begin(greenApiUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["chatId"] = recipientPhoneNumber;
    doc["message"] = message;

    String jsonData;
    serializeJson(doc, jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode == 200) {
        Serial.println("Message sent successfully!");
    } else {
        Serial.print("Error sending message. Response code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

#endif
