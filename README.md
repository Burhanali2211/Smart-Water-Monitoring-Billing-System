# Water Billing System Documentation

## Overview
This project is a **Water Billing System** that monitors water consumption using flow sensors and sends alerts via WhatsApp when predefined thresholds are exceeded. Additionally, it integrates with **ThingSpeak** for real-time data visualization.

## Features
- **Flow Sensor Monitoring**: Measures water usage for two houses.
- **OLED Display**: Shows real-time water consumption.
- **WhatsApp Alerts**: Sends messages when usage crosses predefined limits.
- **ThingSpeak Integration**: Logs water usage data to the cloud for remote monitoring.
- **WiFi Connectivity**: Connects to the internet to send alerts and upload data.

## Components Used
- **ESP32**
- **Flow Sensors** (x2)
- **OLED Display (SSD1306)**
- **WiFi Module (Built-in ESP32)**
- **ThingSpeak Cloud Platform**

## Installation & Setup
### 1. Clone the Repository
```
git clone https://github.com/your-repo/water-billing-system.git
```

### 2. Install Required Libraries
Make sure you have installed the following libraries in the **Arduino IDE**:
- `WiFi.h`
- `HTTPClient.h`
- `ArduinoJson.h`
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`

### 3. Configure Credentials
Update **secrets.h** with your WiFi and API credentials.

### 4. Upload the Code
- Open `main.ino` in **Arduino IDE**
- Connect your ESP32
- Select the correct **board** and **port**
- Click **Upload**

## ThingSpeak Integration
The system uploads data to **ThingSpeak** every interval.
To visualize data:
1. **Create a ThingSpeak Channel**
2. Note your **Channel ID** and **Write API Key**
3. Update `secrets.h` with your API Key.

## WhatsApp Integration
The system uses **Green API** to send alerts via WhatsApp.
- Sign up at [Green API](https://green-api.com/)
- Get your instance ID and API token
- Update `secrets.h` with the details.

## Troubleshooting
- **WiFi Not Connecting**: Check your SSID and password in `secrets.h`.
- **No Data on OLED**: Ensure the display is connected correctly.
- **ThingSpeak Not Updating**: Verify the API key and channel ID.
- **WhatsApp Alerts Not Sending**: Check Green API credentials and internet connectivity.

## License
This project is licensed under the MIT License.

---
**Author**: Burhan Ali


