import QtQuick 2.5

Row {
    property real angle: 0
    property real speed: 0

    spacing: 5
    ColoredImage {
        height: parent.height
        width: parent.height
        rotation: angle
        source: "images/wind-arrow.png"
        overlayColor: weatherCommon.backgroundColor
    }

    Text {
        font.pixelSize: 10
        color: "#999999"
        text: speed.toFixed(1)
    }
}
