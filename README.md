# 🌿 Smart Terrarium Dashboard

![Qt](https://img.shields.io/badge/Qt-6.11-green)
![QML](https://img.shields.io/badge/QML-UI-blue)
![C++](https://img.shields.io/badge/C++-Backend-blue)
![Status](https://img.shields.io/badge/Status-Active-success)
![License](https://img.shields.io/badge/License-Educational-lightgrey)

A modern **Qt Quick (QML + C++) dashboard** for monitoring terrarium environments. This application displays live environmental data, visually highlights safe/dangerous conditions, and is built on a scalable C++ backend.

---

## ✨ Features

### ✅ Live Environment Monitoring
- **Real-time Updates:** Automatic refresh every 5 seconds via a C++ `QTimer` to ensure the dashboard is always current.
- **Dual-Metric Tracking:** Monitors Temperature (°C) and Humidity (%) simultaneously.
- **History Persistence:** The C++ model records data points into a `QVariantList` history, ready for charting and trend analysis.

### 🚦 Smart Visual Alerts & Logic
- **Dynamic Card Colors:** Sensor cards change color based on real-time safety thresholds:
  - 🟢 **Healthy:** Optimal growing conditions.
  - 🟠 **Warning:** Conditions dropping below ideal (Too dry/Too cold).
  - 🔴 **Critical:** High risk (Too hot/Mold risk).
- **Automated Alerts:** A custom `AlertDialog` component triggers automatically with descriptive messages when values cross critical safety boundaries.

### 🎨 Modern Dashboard UI
- **Interactive Design:** Clickable sensor cards allow the user to toggle focus between different environmental metrics.
- **Translucent Overlays:** A sleek, semi-transparent UI layer over a natural background for a modern aesthetic.
- **Responsive Layout:** Optimized for both desktop and touch-screen interfaces.

---

## 🛠 Technical Architecture

### 🧠 C++ Backend (`EnvironmentModel` & `OpenMeteo`)
- **Object-Oriented Model:** Uses a dedicated `EnvironmentModel` class to bridge the gap between networking logic and the UI.
- **Signal/Slot Communication:** Leverages `Q_PROPERTY` and `NOTIFY` signals to ensure the QML interface updates instantly without manual polling.
- **Asynchronous Networking:** Uses `QNetworkAccessManager` to fetch and parse JSON data from the Open-Meteo API without blocking the UI thread.

### 🎨 QML Frontend
- **Modular Components:** Built with reusable components (`SensorCard.qml` and `AlertDialog.qml`) for easy maintenance.
- **Reactive Bindings:** UI states are directly bound to C++ properties, ensuring high performance and low latency.

---

## 🚨 Environmental Safety Thresholds

| Metric | Low (Warning) | Healthy (Optimal) | High (Critical) |
| :--- | :--- | :--- | :--- |
| **Humidity** | < 60% | 60% – 85% | > 85% (Mold Risk) |
| **Temperature**| < 4°C | 4°C – 26°C | > 26°C |

---

## 📁 Project Structure
```
├── main.cpp                     # Application entry point
├── EnvironmentModel.h / .cpp    # Core data model & history logic
├── OpenMeteo.h / .cpp           # API integration & network refresh logic
├── qml/
│   ├── Main.qml                 # Primary UI Layout & alert management
│   ├── SensorCard.qml           # Reusable UI component for environment metrics
│   ├── AlertDialog.qml          # Custom styled popup for critical warnings
│   └── assets/                  # High-quality icons and background imagery
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

This project serves as a robust foundation for a smart terrarium system, demonstrating:
- A clean separation of concerns between **C++ Logic** and **QML Design**.
- Real-time API data consumption and visualization.
- User-centric alert systems for environmental safety.

---

## 🌱 Author

Built with Qt, curiosity, and a love for clean UI and embedded systems.
