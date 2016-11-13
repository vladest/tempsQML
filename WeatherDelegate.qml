import QtQuick 2.6

Column {
    width: indicators.itemHeight*2
    spacing: 0
    Text {
        height: indicators.itemHeight
        width: parent.width
        textFormat: Text.RichText
        text: Qt.formatDateTime(timestamp, "H") + "<sup>" + Qt.formatDateTime(timestamp, "mm") + "</<sup>"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Image {
        height: indicators.itemHeight
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        source: "/Images/weatherIcons/" + weather_codition_icon_id + ".png"
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: weatherModel.kelvin2celsius(temp).toFixed(0) + "\u00B0"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: weatherModel.kelvin2celsius(temp_min).toFixed(0) + "\u00B0"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Item {
        height: indicators.itemHeight
        width: parent.width
        Wind {
            height: indicators.itemHeight/2
            anchors.centerIn: parent
            angle: wind_degrees
            speed: wind_speed
        }
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: pressure.toFixed(0)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: humidity.toFixed(0) + "%"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: rain_3h.toFixed(0) + " mm"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        height: indicators.itemHeight
        width: parent.width
        text: snow_3h.toFixed(0) + " mm"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
