import QtQuick 2.6
import QtGraphicalEffects 1.0

Item {
    width: ListView.view.width / 4

    Rectangle {
        id: border
        anchors.left: parent.left
        height: col.implicitHeight
        width: 2
        color: "#f9f9f9"
    }

    Column {
        id: col
        anchors.left: border.right
        width: parent.width - 2
        height: parent.height
        spacing: 1
        Text {
            width: parent.width
            color: "#999999"
            font.pixelSize: 10
            text: Qt.formatDateTime(timestamp, "ddd")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ColoredImage {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 59
            width: 59
            source: "images/icons/" + weather_codition_icon_id + ".svg"
            overlayColor: weatherCommon.backgroundColor
        }

        Text {
            width: parent.width
            color: "#999999"
            font.pixelSize: 14
            text: weatherCommon.roundup(weatherCommon.convertToCurrentScale(temp_day)) + "\u00B0"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            height: 16
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            Wind {
                height: parent.height
                anchors.centerIn: parent
                angle: wind_degrees
                speed: wind_speed
            }
        }
        Text {
            width: parent.width
            font.pixelSize: 10
            color: "#999999"
            text: weatherCommon.roundup(pressure)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: parent.width
            font.pixelSize: 10
            color: "#999999"
            text: weatherCommon.roundup(humidity) + "%"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        //    Text {
        //        height: indicators.itemHeight
        //        width: parent.width
        //        text: rain_3h.toFixed(0) + " mm"
        //        horizontalAlignment: Text.AlignHCenter
        //        verticalAlignment: Text.AlignVCenter
        //    }
        //    Text {
        //        height: indicators.itemHeight
        //        width: parent.width
        //        text: snow_3h.toFixed(0) + " mm"
        //        horizontalAlignment: Text.AlignHCenter
        //        verticalAlignment: Text.AlignVCenter
        //    }
    }
}
