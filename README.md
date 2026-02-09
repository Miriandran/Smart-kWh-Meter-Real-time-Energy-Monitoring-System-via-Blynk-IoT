# Smart kWh Meter: IoT-Based Real-Time Energy Monitoring

An IoT-based energy monitoring system designed to track electricity consumption in real-time using the **ESP32** microcontroller. This device allows users to monitor their power usage conveniently via a smartphone.

---

## 📝 Project Overview
This project measures electrical parameters such as Voltage, Current, Power, and total Energy (kWh) from household electronic devices. The data is visualized through two channels: a **0.96" OLED Display** for local monitoring and the **Blynk IoT Dashboard** for remote access.

## ✨ Key Features
* **Real-Time Monitoring**: Tracks live Voltage (V), Current (I), and Power (W).
* **Energy Accumulation**: Automatically calculates total energy usage in kWh.
* **Cloud Integration**: Seamless data synchronization with the Blynk IoT platform.
* **Local Visualization**: On-board OLED screen to check status without needing a mobile device.

## 🛠️ Hardware Specifications
* **Microcontroller**: ESP32 (NodeMCU).
* **Current Sensor**: ACS-712.
* **Voltage Sensor**: ZMPT-101B.
* **Display**: 0.96" I2C OLED SSD1306.
* **Firmware**: Developed in Arduino IDE using the **EmonLib** library.

### Pin Mapping (Pinout)
| Component | ESP32 Pin | Type |
| :--- | :---: | :--- |
| **ZMPT-101B (Voltage)** | GPIO 35 | Analog Input |
| **ACS-712 (Current)** | GPIO 34 | Analog Input |
| **OLED SDA** | GPIO 21 | I2C |
| **OLED SCL** | GPIO 22 | I2C |

### Circuit Diagram (Schematic)
Below is the wiring diagram illustrating the connections between the ESP32, sensors, and display.

![Smart kWh Meter Wiring Schematic](Tool-Design/Schematic_Smart-kWh-Meter.png)

*Ensure all high-voltage connections (AC Mains) are insulated properly and handled with extreme caution.*

## 📊 Calculation Basis
The system utilizes the following physics formulas to derive power and energy consumption:
* **Power ($P$):** $$P = V \times I$$
* **Energy ($E$):** $$E = P \times t$$

## 🚀 Configuration Guide
1. Clone this repository.
2. Open the file in the `/firmware` folder using Arduino IDE.
3. Install necessary libraries via Library Manager: `Blynk`, `EmonLib`, and `Adafruit SSD1306`.
4. Insert your Blynk `Auth Token`, WiFi SSID, and Password into the code settings area.
5. Upload the sketch to your ESP32 board.

---
*This project is intended for the development of open-source energy monitoring systems.*
