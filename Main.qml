
// ── Smart Terrarium - Main UI ──────────────────────────────────────────────────
// This file defines the main application window.
// It shows live sensor data (humidity and temperature),
// displays alerts when values go out of range,
// and allows controlling the mist system via MQTT
//  ──────────────────────────────────────────────────────────────────────────────

import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Controls
import QtQuick.Window
import QtCharts


// ── Window configuration ───────────
Window {
    id: root
    minimumWidth:  360
    minimumHeight: 640
    visible: true
    title: qsTr("Smart Terrarium 🪴")

// ── Threshold configuration ───────────
    property real minHumidity: 60
    property real maxHumidity: 85
    property real minTemp: 18
    property real maxTemp: 26

    // Used to freeze humidity value at alert time
    property real alertHumidityValue: NaN

    // Alert condition flags
    property bool humidityLow: environment.valid && environment.humidity < minHumidity
    property bool humidityHigh: environment.valid && environment.humidity > maxHumidity
    property bool tempLow: environment.valid && environment.temp < minTemp
    property bool tempHigh: environment.valid && environment.temp > maxTemp
    property bool alertShown: false

    /*
     * Determines whether a history panel is open.
     * Possible values: "", "humidity", "temperature"
     */
    property string activeMetric: ""

    /* ===== Dynamic UI Colors ===== */

    // Color changes based on humidity state
    property color humidityColor: {
        if (!environment.valid) return "#abd1c6"
        if (environment.humidity > maxHumidity) return "#e16162" // too humid
        if (environment.humidity < minHumidity) return "#f9bc60" // too dry
        return "#abd1c6" // Green
    }

    // Color changes based on temperature sta
    property color tempColor: {
        if (!environment.valid) return "#abd1c6"
        if (environment.temp > maxTemp) return "#e16162" // too warm
        if (environment.temp < minTemp) return "#004643" // too cold
        return "#abd1c6"
    }

    
    /* ============================================================================
     * Background Layer
     * ========================================================================== */

    Item {
        anchors.fill: parent

        // Background image
        Image {
            id: backgroundImage
            anchors.fill: parent
            source: "images/background.jpg"
            fillMode: Image.PreserveAspectCrop
            smooth: true
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
        }


        /* ============================================================================
         * Main Content Area
         * ========================================================================== */

        Item {
            anchors.fill: parent
            anchors.topMargin: 50
            anchors.bottomMargin: 50

        Column {
            id: mainContent
            anchors.centerIn: parent
            spacing: 20
            width: parent.width - 80

            /* ===== Header ===== */
            Column {
                spacing: 4
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: "🪴 Smart Terrarium 🪴"
                    font.pixelSize: 28
                    font.weight: Font.DemiBold
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

                Text {
                    text: "Live environment monitoring"
                    font.pixelSize: 14
                    color: "#fffffe"
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width

                }
            }

            /* ===== Sensor Cards ===== */
            Item {
                width: 560
                anchors.horizontalCenter: parent.horizontalCenter
                height: 300 // fix the row height so layout doesn't jump

                Row {
                    spacing: 32
                    anchors.centerIn: parent
                    // Hide cards when history view is open
                    opacity: activeMetric === "" ? 1.0 : 0.0

                    Behavior on opacity { NumberAnimation {duration:200}}

                /* --- Humidity Card --- */
                SensorCard{
                    label: "Humidity"
                    visible: activeMetric === "" ? 1 : 0
                    enabled: visible
                    numericValue: environment.valid ? environment.humidity : NaN
                    unit: "%"
                    iconSource: humidityHigh ? "humidity/humidity_high.png"
                                : humidityLow ? "humidity/humidity_low.png"
                                              : "humidity/humidity.png"
                    cardColor: humidityColor
                    active: activeMetric === "humidity"
                    onClicked: activeMetric = "humidity"
                }

                /* --- Temperature Card --- */
                SensorCard{
                    label: "Temperature"
                    visible: activeMetric === "" ? 1: 0
                    enabled: visible
                    numericValue: environment.valid ? environment.temp : NaN
                    unit: "°C"
                    iconSource: tempHigh ? "temp/temp_high.png"
                                : tempLow ? "temp/temp_low.png"
                                :"temp/temperature.png"
                    cardColor: tempColor
                    active: activeMetric === "temperature"
                    onClicked: activeMetric = "temperature"

                }
            }

                /* ===== History Chart Overlay ===== */
                    HistoryChart {
                        anchors.fill: parent
                        visible: activeMetric !== ""
                        dateLabels: environment.dateHistory
                        anchors.horizontalCenter:parent.horizontalCenter
                        metric: activeMetric // humidity or temp
                        dataPoints: activeMetric == "humidity" ? environment.humidityHistory : environment.tempHistory
                        color: activeMetric == "humidity" ? humidityColor : tempColor
                        onCloseRequested: activeMetric = ""
                    }
                }

            /* ===== Instruction Text ===== */
            Text {
                text: "Tap a card to view history 💬"
                font.pixelSize: 12
                font.bold: true
                color: "#fffffe"
                opacity: activeMetric ==="" ? 0.8 : 0
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter

                Behavior on opacity {
                    NumberAnimation{
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }


            /* ===== Status Bar ===== */
            Rectangle{
                width: parent.width
                height: 44
                radius: 12
                color: "transparent"

                Row {
                    anchors.centerIn: parent
                    spacing: 32

                    Text {

                        text: environment.humidity > 85 ? "Mold Risk" : "Mold Risk: Low"
                        font.pixelSize: 17
                        font.bold: true
                        color: environment.humidity > 85 ? "#c62828" : "#fffffe"
                    }

                    Text {
                        text: qsTr("Mode: Online")
                        font.pixelSize: 17
                        font.bold: true
                        color: "#fffffe"
                    }
                }

            }


            /* ===== Metadata ===== */
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Source: " + environment.source
                font.pixelSize: 12
                color: "#fffffe"
            }

            Item {
                height: 1
                width: 1
            }

            /* ===== Mist Control ===== */
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                height: 70
                width: 520
                radius: 20
                color: "transparent"

                Column {
                    anchors.centerIn: parent
                    spacing: 8

                    Text {
                        text: mqtt.connected ? "Connected ✅" : "Disconnected ❗"
                        font.pixelSize: 11
                        font.bold: true
                        color: "#fffffe"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Button {
                        text: mqtt.mistOn ? "💦​Mist ON" : "💨 Mist OFF"
                        width: 120
                        height: 40
                        enabled: mqtt.connected
                        opacity: mqtt.connected ? 1.0 : 0.5


                        background: Rectangle {
                            radius: 10
                            color: mqtt.mistOn ? "#abd1c6" : "#f9bc60"
                            Behavior on color {

                                ColorAnimation { duration: 300 }}
                        }

                        contentItem: Text {
                            text: parent.text
                            color: "#001e1d"
                            font.pixelSize: 15
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: mqtt.setMist(!mqtt.mistOn)

                    }
                }
            }
        }
    }
}

    
    /* ============================================================================
     * Alerts
     * ========================================================================== */

    AlertDialog{
        id: alertDialog
    }

    // Humidity alerts
    onHumidityHighChanged: {
        if (humidityHigh){
            alertDialog.titleText = "Mold Risk​🔴​"
            alertDialog.messageText = "Humidity is too high (" + Math.round(environment.humidity) +
                    "%).\nRisk of mold growth."
            alertDialog.open()
        }

        if (!humidityHigh){
            alertShown = false
        }
    }

    onHumidityLowChanged: {
        if (humidityLow){
            alertHumidityValue = environment.humidity
            alertDialog.titleText = "Low Humidity 🟠​"
            alertDialog.messageText = "Humidity is too low (" + Math.round(environment.humidity) +
                    "%).\nPlants may dry out."
            alertDialog.open()
        }

        if (!humidityLow){
            alertShown = false
        }
    }

    // Temperature alerts
    onTempHighChanged: {
        if (tempHigh){
            alertDialog.titleText = "High Temperature 🥵"
            alertDialog.messageText = "Temperature is too high (" + environment.temp.toFixed(1) +
            "°C)."
            alertDialog.open()
        }
        if (!tempHigh){
            alertShown= false
        }
    }

    onTempLowChanged: {
        if (tempLow){
            alertDialog.titleText = "Low Temperature 🥶"
            alertDialog.messageText = "Temperature is too low (" + environment.temp.toFixed(1) +
            "°C)."
            alertDialog.open()
        }
        if (!tempLow){
            alertShown = false
        }
    }

}
