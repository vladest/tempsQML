import QtQuick 2.5

Row {
    property real angle: 0
    property real speed: 0

    spacing: 5
    Image {
        height: parent.height
        width: parent.height
        rotation: angle
        source: "Images/wind-arrow.png"
    }
    Text {
        text: speed
    }
}
