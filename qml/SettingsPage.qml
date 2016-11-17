import QtQuick 2.5
import QtQuick.Controls 2.2
import weathermodel 1.0

Item {

    Column {
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        spacing: 10
        Row {
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5
            Image {
                source: "images/app.png"
                width: 45
                height: 45
            }
            Text {
                text: "tempsQML"
                font.pixelSize: 32
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                id: smallSun
                source: "images/IconTemplate@2x.png"
                PropertyAnimation {
                    target: smallSun
                    running: true; property: "rotation"
                    from: 0; to: 359; duration: 5000; loops: Animation.Infinite }
            }
        }

        Text {
            text: qsTr("City")
            color: "#999999"
            font.pixelSize: 14
        }
        TextField {
            width: parent.width
            text: WeatherModel.cityName + ", " + WeatherModel.countryID
            font.pixelSize: 16
            background: Rectangle {
                width: parent.width
                height: 2
                y: parent.height - 2
                color: "#999999"
            }
        }

        Row {
            width: parent.width
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Temperature scale")
                color: "#999999"
            }
            Item {
                width: 20; height: 1
            }

            ButtonGroup { id: scaleGroup }

            SettingsRadio {
                id: cels
                font.pixelSize: 18
                text: "\u00B0C"
                checked: WeatherModel.tempScale === Weather.Celsium
                ButtonGroup.group: scaleGroup
            }
            SettingsRadio {
                id: fahr
                font.pixelSize: 18
                text: "\u00B0F"
                checked: WeatherModel.tempScale === Weather.Fahrenheit
                ButtonGroup.group: scaleGroup
            }
        }
        SettingsCheck {
            id: systray
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather in menubar")
            checked: WeatherModel.menuBarWeather
        }
        SettingsCheck {
            id: startup
            width: parent.width
            leftPadding: 0
            text: qsTr("Start at login")
            checked: WeatherModel.runAtStartup
        }
        SettingsCheck {
            id: anim
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather animation")
            checked: WeatherModel.showAnimation
        }
        Row {
            width: parent.width
            spacing: 10
            SettingsButton {
                width: parent.width/2 - 5
                text: qsTr("Apply")
                onClicked: {
                    WeatherModel.menuBarWeather = systray.checked
                    WeatherModel.runAtStartup = startup.checked
                    WeatherModel.showAnimation = anim.checked
                    WeatherModel.tempScale = fahr.checked ? Weather.Fahrenheit : Weather.Celsium
                }
            }
            SettingsButton {
                width: parent.width/2 - 5
                text: qsTr("Quit")
                onClicked: Qt.quit()
            }
        }
    }
}
