import QtQuick
import QtQuick.Controls


/*
 * SensorCard
 *
 * A reusable UI component that displays a single environmental metric
 * (e.g. humidity or temperature).
 *
 * The card shows:
 *  - An icon
 *  - A label
 *  - A numeric value with unit
 *  - Hover and active animations
 *
 * Clicking the card typically opens a history view.
 */

Item {
    id: root
    width: 150
    height: 180

    /* ===== Public properties ===== */

    // Sensor label (e.g. "Humidity", "Temperature")
    property string label: ""
    property string unit: ""
    // Icon image source
    property url iconSource: ""
    property color cardColor: "#3a8f3a"
    // Current numeric value displayed on the card
    property real numericValue: 0
    // Indicates whether this card is currently active/selected
    property bool active: false
    // True while mouse is hovering over the card
    property bool hovered: mouse.containsMouse

    signal clicked()

 
    /* ===== Shadow layer ===== */

    // Creates a soft shadow effect behind the card
    Rectangle{
        anchors.fill: parent
        radius: 20
        y: active ? 16 : hovered ? 13: 10
        color: hovered || active ? "#30000000" : "#25000000"
    }

    /* ===== Card body ===== */
    Rectangle{
        id: card
        anchors.fill: parent
        radius: 20
        color: cardColor
        border.width: active ? 2 : 0
        border.color: "white"
        // Slight scale animation for hover/active states
        scale: active ? 1.04 : hovered ? 1.02 : 1.0

        Behavior on scale {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutCubic
            }
        }

        Behavior on border.width {
            NumberAnimation{
                duration: 150
            }
        }

        /* ===== Card content ===== */
        Column {
            anchors.centerIn: parent
            spacing: 10

            // Sensor icon
            Image {
                source: iconSource
                width: 36
                height: 36
                fillMode: Image.PreserveAspectFit
                // visible: iconSource !== ""
            }

            Text {
                text: label
                font.pixelSize: 14
                color: "#f5f5f5"
                opacity: 0.9
            }

            // Sensor value display
            Text {
                // visible:true
                text: isNaN(numericValue) ? "--" : Math.round(numericValue) + " " + unit
                font.pixelSize: 20
                font.weight: Font.Bold
                color: "white"

                // Smooth text update animation
                Behavior on text {
                    NumberAnimation{
                        duration: 30
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            // Hint text shown on hover
            Text {
                text: "View history"
                font.pixelSize: 11
                color: "#fffffe"
                opacity: hovered ? 0.8 : 0
                horizontalAlignment: Text.AlignHCenter

                Behavior on opacity {
                    NumberAnimation {
                        duration: 150
                    }
                }
            }
        }

}
    /* ===== Mouse handling ===== */
    MouseArea{
        id: mouse
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onClicked: root.clicked()
    }

    /* ===== Smooth property transitions =====
    Behavior on active {
        NumberAnimation{
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on numericValue {
        NumberAnimation{
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on cardColor {

        ColorAnimation {
            duration: 400
            easing.type: Easing.InOutQuad
        }
    }
}
