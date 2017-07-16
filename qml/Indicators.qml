import QtQuick 2.7

Column {
    property real itemHeight: 16
    spacing: 0

    Item {
        height: parent.itemHeight
        width: height
        Image {
            height: parent.height/2
            width: parent.width/2
            anchors.centerIn: parent
            source: "Images/round-clock-icon.png"
        }
    }
    //empty item
    Item {
        height: parent.itemHeight
        width: height
    }
    Text {
        height: parent.itemHeight
        width: height
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "C\u00B0"
        verticalAlignment: Text.AlignVCenter
    }
    Item {
        height: parent.itemHeight
        width: height
        Image {
            height: parent.height/2
            width: parent.width/2
            anchors.centerIn: parent
            source: "Images/termometer.png"
        }
    }
    Text {
        height: parent.itemHeight
        width: height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "kt"
    }
    Text {
        height: parent.itemHeight
        width: height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "hPa"
    }
    Item {
        height: parent.itemHeight
        width: height
        Image {
            height: parent.height/2
            width: parent.width/2
            anchors.centerIn: parent
            source: "Images/humidity.png"
        }
    }
    Item {
        height: parent.itemHeight
        width: height
        Image {
            height: parent.height/2
            width: parent.width/2
            anchors.centerIn: parent
            source: "Images/umbrella.png"
        }
    }
    Item {
        height: parent.itemHeight
        width: height
        Image {
            height: parent.height/2
            width: parent.width/2
            anchors.centerIn: parent
            source: "Images/snow.png"
        }
    }
}
