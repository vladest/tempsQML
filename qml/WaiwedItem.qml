import QtQuick 2.7

Rectangle {
    id: root
    property real yPage: 0
    color: WeatherModel.backgroundColor
    signal settingsClicked

    SequentialAnimation {
        id: rainAnimation
        running: WeatherModel.currentWeather.weather_codition_icon_id === "11d" ||
                 WeatherModel.currentWeather.weather_codition_icon_id === "11n"
        loops: Animation.Infinite

        PropertyAnimation {
            target: root
            property: "opacity"
            from: 0
            to: 1
            duration: 500;
            easing.type: Easing.OutElastic
        }

        //random pause between 1 and 7 seconds
        PauseAnimation {
            duration: Math.ceil(Math.random() * 6000) + 1000
        }
    }

    Behavior on color { PropertyAnimation { duration: 500; } }
    Text {
        id: clock
        height: 20
        z: 100
        anchors { right: parent.right; top: parent.top; rightMargin: 10; topMargin: 10; }
        font.pixelSize: 12
        color: "white"
        Timer {
            interval: 5000
            repeat: true
            triggeredOnStart: true
            running: true
            onTriggered: {
                clock.text = Qt.formatDateTime(new Date(), "hh:mm")
            }
        }
    }
    MenuClose {
        id: menu
        height: 20
        width: 20
        z: 1000
        onStateChanged: {
            settingsClicked()
        }

        anchors { left: parent.left; top: parent.top; leftMargin: 10; topMargin: 10; }
        MouseArea {
            anchors.fill: parent
            onClicked: menu.state = (menu.state == "menu" ? "back" : "menu")
        }
    }

    Image {
        id: img
        width: parent.width
        y: root.yPage - 40// + sourceSize.height
        source: "images/background.png"
        Behavior on y { PropertyAnimation { duration: 630 } }
        //onYChanged: console.log(y)
    }
    Rectangle {
        color: "white"
        width: parent.width
        height: parent.height - img.y - img.sourceSize.height
        anchors.bottom: parent.bottom
    }
}
