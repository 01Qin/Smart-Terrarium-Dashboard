import QtQuick
import QtQuick.Controls
import QtCharts


/*
 * HistoryChart
 *
 * This component displays a historical line chart for either
 * humidity or temperature values using QtCharts.
 * It supports animated appearance and dynamic data updates.
 */
Item {
    id: root
    // Determines which metric is shown ("humidity" or "temperature").
    property string metric: "humidity"

    /*
     * Numeric values to be plotted on the chart.
     * Each value corresponds to one date label.
     */
    property var dataPoints: []
    property color color: "#abd1c6"

    /*
     * List of date strings corresponding to the data points.
     */
    property var dateLabels: []
    signal closeRequested()

    /* ===== Fade-in animation ===== */
    // animate in
    opacity: visible ? 1 : 0
    Behavior on opacity {
        NumberAnimation {
            duration: 250
        }
    }

    /* ===== Background and layout container ===== */
    Rectangle {
        anchors.fill: parent
        radius: 20
        color: "#cc000000"

    /* ===== Close button ===== */
        Rectangle{
            id: closeButton
            width: 28
            height: 28
            radius: 6
            color: "#001e1d"
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 10

        // Close icon
        Text {
            text: qsTr("x")
            color: "#e8e4e6"
            anchors.centerIn: parent
            font.pixelSize: 16
            font.bold: true
        }
            // Click handling
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.closeRequested()
            }
        }
         /* ===== Chart title ===== */
        Text {
            id: chartTitle

            // Title depends on selected metric
            text: root.metric === "humidity" ? "Humidity History (%)" : "Temperature History (°C)"
            color: "white"
            font.pixelSize: 13
            font.bold: true
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 12
        }

        /* ===== Chart area ===== */
        ChartView {
            anchors.top: chartTitle.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 8
            backgroundColor: "transparent"
            plotAreaColor: "transparent"
            legend.visible: false
            antialiasing: true
            margins.top: 0
            margins.bottom: 24
            margins.left: 0
            margins.right: 0

            /* ===== X Axis (dates) ===== */
            DateTimeAxis {
                id: axisX
                format: "MMM d" // Example: Apr 15

                labelsColor: "#fffffe"
                labelsFont.pixelSize: 10
                labelsAngle: -45
                gridLineColor: "#33ffffff"

            }

            /* ===== Y Axis (values) ===== */
            ValueAxis {
                id: axisY
                min: root.metric == "humidity" ? 0 : -10
                max: root.metric == "humidity" ? 100 : 40
                labelsColor: "#fffffe"
                labelsFont.pixelSize: 10
                gridLineColor: "#33ffffff"
                tickCount: 5
            }

            /* ===== Data series ===== */
            LineSeries {
                id: series
                axisX: axisX
                axisY: axisY
                width: 2
                color: "white"
            }

            // Build the chart once the component is ready
            Component.onCompleted: root.rebuildSeries()
        }
    }
    
    /* ===== Data handling logic ===== */

    /*
     * Rebuilds the line series whenever data changes.
     * Converts date strings into timestamps and updates axis ranges.
     */
    function rebuildSeries(){
        series.clear()

        if(!dataPoints || dataPoints.length === 0)
            return

        var minTime = null
        var maxTime = null


        for (var i =0; i < dataPoints.length; i++){
            // Use matching date label if availab
            var dateStr = (dateLabels && dateLabels.length > i) ? dateLabels[i] : ""
            var date = new Date(dateStr)

            // Skip invalid dates
            if(isNaN(date.getTime())) continue

            // Track time range
            if (!minTime || date < minTime) minTime = date
            if (!maxTime || date > maxTime) maxTime = date
            series.append(date.getTime(), dataPoints[i])
        }

        // Update X axis range if valid
        if(minTime && maxTime){
            axisX.min = minTime
            axisX.max = maxTime
        }
    }

    /* ===== React to data changes ===== */
    onDataPointsChanged: rebuildSeries()
    onMetricChanged: rebuildSeries()
    onDateLabelsChanged: rebuildSeries()
}
