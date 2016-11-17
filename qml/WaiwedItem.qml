import QtQuick 2.7
import QtQuick.Particles 2.0
import weathermodel 1.0

Rectangle {
    id: root
    property real yPage: 0
    color: WeatherModel.backgroundColor
    signal settingsClicked
    state: "none"

    //flash effect
    SequentialAnimation {
        id: rainAnimation
        //running only on certain weather conditions
        running: WeatherModel.currentWeather.weather_codition_icon_id === "11d" ||
                 WeatherModel.currentWeather.weather_codition_icon_id === "11n"
        loops: Animation.Infinite

        //the idea is to quick change opacity...
        PropertyAnimation {
            target: root
            property: "opacity"
            from: 0
            to: 1
            duration: 500;
            easing.type: Easing.OutElastic
        }

        //..and then random pause between 1 and 7 seconds between flashes
        PauseAnimation {
            duration: Math.ceil(Math.random() * 6000) + 1000
        }
    }

    //setup snow or rain particles
    //using particles subsystem
    Connections {
        target: WeatherModel
        onCurrentWeatherChanged: {
            SystemTrayIcon.setNewIcon("icons/"+WeatherModel.currentWeather.weather_codition_icon_id + ".svg")
            SystemTrayIcon.toolTip = WeatherModel.currentWeather.weather_codition_description
            SystemTrayIcon.show()
            SystemTrayIcon.showMessage(WeatherModel.currentWeather.weather_codition_name,
                                       WeatherModel.currentWeather.weather_codition_description
                                       + " " + WeatherModel.roundup(WeatherModel.kelvin2celsius(WeatherModel.currentWeather.temp))
                                       + "\u00B0" + WeatherModel.temperatureScale, 0, 3000)
            if (WeatherModel.currentWeather.weather_codition_name === 'Rain') {
                if (WeatherModel.currentWeather.weather_codition_description.indexOf('light') !== -1) {
                    rainEmitter.emitRate = 5
                } else if (WeatherModel.currentWeather.weather_codition_description.indexOf('heavy') !== -1) {
                    rainEmitter.emitRate = 20
                } else {
                    rainEmitter.emitRate = 10
                }
                root.state = "rain"
            } else if (WeatherModel.currentWeather.weather_codition_name === 'Snow') {
                snowEmitter.emitRate = 10
                root.state = "snow"
            }
            else {
                root.state = "none"
            }
        }
    }

    ParticleSystem {
        id: sysRain
        running: root.state === "rain"
    }

    Emitter {
        id: rainEmitter
        system: sysRain
        emitRate: 10
        lifeSpan: 2000
        velocity: AngleDirection {angle: 90; magnitude: 500;}
        width: parent.width
        height: 200
    }

    ParticleSystem {
        id: sysSnow
        running: root.state === "snow"
    }

    Emitter {
        id: snowEmitter
        system: sysSnow
        emitRate: 20
        lifeSpan: 5000
        //snow flakes slower than rain drops
        velocity: AngleDirection {angle: 90; magnitude: 50;}
        width: parent.width
        height: 300
    }

    ItemParticle {
        id: snowparticles
        anchors.fill: parent
        system: sysSnow
        delegate: SnowFlake {}
        fade: true
    }

    ItemParticle {
        id: rainparticles
        anchors.fill: parent
        system: sysRain
        delegate: RainDrop {}
        fade: true
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
