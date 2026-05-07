# 🌿 Smart Terrarium Dashboard

![Qt](https://img.shields.io/badge/Qt-6.11-green)
![QML](https://img.shields.io/badge/QML-UI-blue)
![C++](https://img.shields.io/badge/C++-Backend-blue)
![MQTT](https://img.shields.io/badge/MQTT-HiveMQ-purple)
![Status](https://img.shields.io/badge/Status-Active-success)
![License](https://img.shields.io/badge/License-Educational-lightgrey)

A modern **Qt Quick (QML + C++) dashboard** for monitoring terrarium environments in real time. Displays live weather data, highlights safe/dangerous conditions with dynamic visuals, controls a mist system via MQTT, and visualizes 10-day environmental history — all backed by a clean C++ data model.

---

## ✨ Features

### ✅ Live Environment Monitoring
- **Real-time Updates:** Automatic refresh every 60 seconds via `QTimer`, fetching current temperature and humidity from the Open-Meteo API.
- **Correct Hour Matching:** Parses the Open-Meteo hourly time array to always display the value for the current UTC hour — not just index 0.
- **Dual-Metric Tracking:** Monitors Temperature (°C) and Humidity (%) simultaneously with `Q_PROPERTY` bindings.
- **Data Source Attribution:** Displays the active data source (e.g. "Open-Meteo") in the UI footer.

### 📈 10-Day History Charts
- **Interactive History Panel:** Tap a sensor card to reveal a `ChartView`-based line chart showing the past 10 days of data.
- **Real Date Labels:** X-axis uses `DateTimeAxis` with actual calendar dates (e.g. "Apr 22", "Apr 30") parsed from the API `time` array.
- **Smooth Transitions:** Chart fades in and out over the sensor cards without shifting any other UI elements.
- **Metric Switching:** Switching between humidity and temperature cards instantly redraws the chart with the correct data and color.
- **History Isolation:** History data is stored separately from live values using dedicated `appendTempHistory` / `appendHumidityHistory` slots, preventing history fetches from corrupting the live display.

### 💧 MQTT Mist Control
- **Remote Control:** Publishes `ON` / `OFF` commands to the `terrarium/mist` topic on HiveMQ public broker.
- **Live State Sync:** Subscribes to the same topic so external commands (e.g. from `mosquitto_pub`) are reflected in the UI instantly.
- **Connection Status:** Displays a live 🟢 Connected / 🔴 Disconnected indicator next to the mist button.
- **Retained Messages:** Publishes with retain flag so the broker remembers the last mist state across reconnects.
- **Echo Guard:** Ignores its own echoed messages to prevent the button from flickering on click.

### 🚦 Smart Visual Alerts
- **Dynamic Card Colors:** Sensor cards change color based on configurable safety thresholds:
  - 🟢 **Healthy:** Optimal growing conditions
  - 🟠 **Warning:** Too dry or too cold
  - 🔴 **Critical:** Mold risk or overheating
- **Dynamic Icons:** Card icons swap automatically to match the current condition state (high / normal / low variants).
- **Automated Alert Dialogs:** A custom `AlertDialog` fires with descriptive messages when values cross thresholds, with severity-based styling (yellow warning / red critical).
- **Valid-State Guard:** Alerts only fire after both temperature and humidity have been successfully received, preventing false alarms on startup.

### 🎨 Modern Dashboard UI
- **Overlay History:** History chart overlays the sensor cards in the same layout slot — nothing jumps or shifts.
- **Fade Animations:** Cards and charts fade in/out smoothly using `Behavior on opacity`.
- **Translucent Panels:** Semi-transparent dark overlays over a natural forest background.
- **Responsive Layout:** Optimized for both desktop and touch-screen interfaces with `minimumWidth`/`minimumHeight` constraints.

---

## 🛠 Technical Architecture

### 🧠 C++ Backend

| Class | Responsibility |
| :--- | :--- |
| `EnvironmentModel` | Core data model. Holds live values, history lists, and validity state. Exposes all data to QML via `Q_PROPERTY`. |
| `OpenMeteo` | Async network layer. Fetches current hourly data and 10-day daily history from Open-Meteo. Manages a `QTimer` for periodic refresh. |
| `MqttController` | MQTT layer. Wraps `QMqttClient` to publish/subscribe mist commands. Exposes `mistOn` and `connected` state to QML. |

- **Signal/Slot Communication:** All model updates propagate to QML automatically via `NOTIFY` signals — no manual polling.
- **Asynchronous Networking:** `QNetworkAccessManager` fetches JSON without blocking the UI thread.
- **NaN Initialization:** `m_humidity` and `m_temp` initialize to `qQNaN()` so `valid` only becomes `true` once both real values have arrived.

### 🎨 QML Frontend

| Component | Role |
| :--- | :--- |
| `Main.qml` | Root window, layout, alert wiring, MQTT button, threshold logic |
| `SensorCard.qml` | Reusable animated metric card with hover, active, and color states |
| `HistoryChart.qml` | `ChartView` overlay with `DateTimeAxis`, `ValueAxis`, and `LineSeries` |
| `AlertDialog.qml` | Modal popup with severity-based icon, color, and dismiss button |

---

## 🚨 Environmental Safety Thresholds

| Metric | Low (Warning) | Healthy (Optimal) | High (Critical) |
| :--- | :--- | :--- | :--- |
| **Humidity** | < 60% | 60% – 85% | > 85% (Mold Risk) |
| **Temperature** | < 18°C | 18°C – 26°C | > 26°C |

---

## 📁 Project Structure
```
├── main.cpp                     # Application entry point
├── EnvironmentModel.h / .cpp    # Core data model & history logic
├── OpenMeteo.h / .cpp           # API integration & network refresh logic
├── mqtt_controller.h / .cpp     # MQTT publish/subscribe, mist state management
├── qml/
│   ├── Main.qml                 # Primary UI Layout & alert management
│   ├── SensorCard.qml           # Reusable UI component for environment metrics
│   ├── AlertDialog.qml          # Custom styled popup for critical warnings
    ├── HistoryChart.qml         # 10-day line chart with DateTimeAxis
│   └── images/                  # High-quality icons and background imagery
│       ├── background.jpg
│       └── humidity/            # humidity.png, humidity_high.png, humidity_low.png
        └── temp/                # temperature.png, temp_high.png, temp_low.png
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

This project demonstrates:
- Clean separation of concerns between **C++ logic** and **QML design**
- Real-time API consumption with correct hour indexing and history charting
- MQTT device control with retained state and echo filtering
- Robust UI patterns: overlay charts, animated transitions, valid-state guards

---

## 🌱 Author

Built with Qt, curiosity, and a love for clean UI and embedded systems.
