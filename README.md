# 🌿 Smart Terrarium Dashboard

![Qt](https://img.shields.io/badge/Qt-6.11-green)
![QML](https://img.shields.io/badge/QML-UI-blue)
![C++](https://img.shields.io/badge/C++-Backend-blue)
![Status](https://img.shields.io/badge/Status-Active-success)
![License](https://img.shields.io/badge/License-Educational-lightgrey)


A modern **Qt Quick (QML + C++) dashboard** for monitoring terrarium.  
The application displays live **humidity** and **temperature** data, visually highlights safe and dangerous conditions, and provides a clean, touch‑friendly user interface inspired by real industrial dashboards.

The project currently uses **Open‑Meteo** as a simulated data source and is designed to seamlessly transition to **real sensors via MQTT or Raspberry pi** in the future.

---

## ✨ Features

### ✅ Live Environment Monitoring
- Real‑time **humidity (%)** and **temperature (°C)** updates
- Automatic refresh using a C++ backend timer
- Data sourced from **Open‑Meteo API**

### 🎛 Interactive Dashboard UI
- Clickable **Humidity** and **Temperature** cards
- Only the selected card displays live values (focused view)
- Smooth animations for value and color changes

### 🚦 Smart Visual Alerts
- Cards automatically change color based on thresholds:
  - 🟢 Healthy
  - 🟠 Too Low
  - 🔴 Too High / Mold Risk
- Status bar shows current environment state
- Styled **AlertDialog** pops up for critical conditions

### 🎨 Modern Design
- Background image with transparent UI overlays
- Rounded, card‑based layout
- Consistent colors and typography
- Optimized for desktop and touch screens

### 🧠 Clean Architecture
- Clear separation between:
  - **C++ backend (logic & networking)**
  - **QML frontend (UI & interaction)**
- Ready for future MQTT / IoT integration

---

## 🖼 UI Overview

- Background image visible through transparent UI elements
- Centered header with title and subtitle
- Two main sensor cards:
  - 💧 Humidity
  - 🌡 Temperature
- Status information (Mold Risk, Mode, Connection)
- Control buttons (Simulation / Auto Mode / Mist ON/OFF)

---

## 🌐 Data Source

The project uses the **Open‑Meteo API** to simulate real sensor data.

### Used fields
- Temperature: `current.temperature_2m`
- Humidity: `current.relative_humidity_2m`

### Example API endpoint

## 🚨 Alert Thresholds
A timer in `OpenMeteo.cpp` periodically refreshes the data and updates  
the shared environment model.

---

### 💧 Humidity
- **Too Low:** `< 60 %`  
- **Healthy:** `60 – 85 %`  
- **Mold Risk:** `> 85 %`  

### 🌡 Temperature
- **Too Cold:** `< 18 °C`  
- **Healthy:** `18 – 26 °C`  
- **Too Hot:** `> 26 °C`  

These thresholds control:
- Card color changes  
- Status indicators  
- Alert dialog pop‑ups  

---

## 📁 Project Structure
```
├── main.cpp
├── EnvironmentModel.h / .cpp
├── OpenMeteo.h / .cpp
├── qml/
│   ├── Main.qml
│   ├── SensorCard.qml
│   ├── AlertDialog.qml
│   └── assets/
│       ├── backgrounds/
│       └── icons/
└── README.md
```

---

## 🛠 Build & Run

### Requirements
- **Qt 6.x**  
- **Qt Creator** (recommended)  
- A supported C++ compiler

---

## 🎯 Project Purpose

This project serves as:
- A learning project for **Qt Quick + C++**  
- A foundation for a real **IoT terrarium system**  
- A reusable dashboard architecture for sensor‑based applications  

---

## 🌱 Author

Built with Qt, curiosity, and a love for clean UI and embedded systems.
