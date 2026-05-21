# 🚀 XIAO ESP32-S3 Sense Development Kit for IoT Applications

![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)
![C++](https://img.shields.io/badge/Language-C++-blue)
![React](https://img.shields.io/badge/Frontend-React%20%7C%20TailwindCSS-61DAFB)
![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

## Overview
The **XIAO ESP32-S3 Sense Dev Kit** is a comprehensive, highly modular platform designed to streamline Internet of Things (IoT) prototyping. Built around the powerful dual-core **XIAO ESP32-S3 Sense**, this project provides a **Platform Abstraction Layer (PAL)** and **object-oriented C++ drivers** for a wide range of hardware peripherals.

This project bridges the gap between low-level hardware abstraction and cloud-based IoT solutions, culminating in a fully functional **Smart Home Prototype** featuring real-time environmental monitoring, secure facial recognition access, and cloud automation via the CoreIOT platform.

---

## Key Features

*   **Custom Platform Abstraction Layer (PAL):** A scalable abstraction layer for GPIO, I2C, UART, and RS-485 (Modbus RTU) interfaces.
*   **Real-Time Multitasking:** Utilizes **FreeRTOS** for concurrent, non-blocking task scheduling (e.g., sensor polling, network communication, and actuator control).
*   **Advanced IoT Connectivity:** Seamless integration of Wi-Fi, Bluetooth Low Energy (BLE), and MQTT stacks for reliable, bidirectional cloud communication.
*   **Enterprise Cloud Integration:** Fully compatible with **CoreIOT (ThingsBoard)**, featuring custom Rule Chains, RPC (Remote Procedure Call) commands, and real-time telemetry dashboards.
*   **Over-The-Air (OTA) Updates:** Supports remote firmware flashing directly from the CoreIOT dashboard via MQTT chunk downloading.
*   **Onboard Web Server (Captive Portal):** Features an embedded React and TailwindCSS web interface served via LittleFS for dynamic Wi-Fi and MQTT configuration.
*   **Edge AI Vision:** Integrates the **HuskyLens AI camera** for localized facial recognition and secure automated door access.

---

## Hardware & Supported Peripherals

The BSP abstracts hardware interactions into Object-Oriented classes, making it incredibly simple to initialize and manage devices.

| Interface | Supported Devices & Modules | Use Case in Demo |
| :--- | :--- | :--- |
| **GPIO** | PIR Sensor, Light Sensor, Button, RC Servo, Mini Fan, RGB LEDs | Motion detection, lighting, door actuation, cooling |
| **I2C** | DHT20, LCD 16x2, M5 Unit ENV-IV, Unit 4-Relay, AC Measure | Climate monitoring, power metrics, high-power switching |
| **UART** | HuskyLens AI Camera | Secure facial recognition access |
| **RS-485** | ES-Soil-7-in-1 Sensor (via Modbus RTU) | Outdoor/Garden monitoring (pH, Moisture, NPK) |

---

## Software Architecture

The software architecture is designed using industry best practices, adhering to **LLVM coding standards** (managed via `clang-format`) and utilizing the **GitFlow** branching model.

### 1. Embedded Firmware (C++ / PlatformIO)
*   Developed in **VS Code** with the **PlatformIO** extension.
*   The architecture is divided into the **Sensing Layer**, **IoT Gateway Layer**, **Data Processing Layer**, and **Application Layer**.
*   Each peripheral driver is isolated into its own reusable class (e.g., `dht20.cpp`, `sht4x.cpp`) with error handling.

### 2. Frontend Configuration Portal (React)
*   A responsive Single Page Application (SPA) built with **React** and **TailwindCSS**.
*   Hosted directly on the ESP32-S3 via the `WebServer` library and `LittleFS`.
*   Features Light/Dark mode toggling and securely saves user network credentials into Non-Volatile Storage (NVS).

---

## Demonstration: Smart Home Prototype

To validate the development kit, a complete Smart Home system was deployed.

*   **Multi-Room Dashboards:** Real-time CoreIOT telemetry dashboards for the Living Room, Bedroom, Kitchen, and Outdoor Garden.
*   **Security System:** The HuskyLens AI camera automatically scans for authorized faces and commands the servo motor to unlock the door upon recognition.
*   **Automated Alarms:** Server-side Rule Chains automatically trigger critical alerts via the web and mobile app if temperature or humidity exceeds predefined safety thresholds.
*   **Weather API Integration:** A custom rule chain fetches real-time external weather data via REST API to complement local sensor readings.

### [Demo Video](https://youtu.be/bsBi9iFxKns)
---

## Getting Started

### Prerequisites
*   [VS Code](https://code.visualstudio.com/) installed.
*   [PlatformIO IDE](https://platformio.org/) extension installed in VS Code.
*   Node.js (for modifying the React web portal).

### Installation & Build

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yshic/thesis_mdt266_iotdevkit.git
   cd thesis_mdt266_iotdevkit
2. **Upload the Web Portal (LittleFS):**
- Pre-build the React app (or use the provided build/ folder).
- In PlatformIO, run the Upload Filesystem Image task to flash the web files to the ESP32.
- CLI Command: pio run --target uploadfs
3. **Build and Upload the Firmware:**
- Connect your XIAO ESP32-S3 Sense to your PC via USB-C.
- Click the "Upload" button in PlatformIO.
4. **Initial Setup (Captive Portal):**
- On first boot, the device will host a Wi-Fi Access Point named XIAO-SMART-HOME.
- Connect to this network and navigate to 192.168.4.1 in your browser.
- Enter your local Wi-Fi credentials and MQTT broker details.
- The device will save these, reboot, and connect to the cloud automatically.

--------------------------------------------------------------------------------
### Author
Nguyen Le Anh Tuan
LinkedIn: linkedin.com/in/yshic
GitHub: github.com/yshic
Email: tuanl799@gmail.com
This project was completed as a Graduation Thesis for the Faculty of Computer Science and Engineering at the Ho Chi Minh City University of Technology (HCMUT).

# [Documentation](https://yshic.github.io/thesis_dev_kit_docs)
