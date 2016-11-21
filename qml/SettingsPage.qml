import QtQuick 2.5
import QtQuick.Controls 2.2
import weathermodel 1.0

Item {

    StackView.onStatusChanged: {
        if (StackView.status === StackView.Activating) {
            cityEdit.loadCities()
        } else if (StackView.status === StackView.Deactivating) {
            cityEdit.saveCities()
        }
    }

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
        EditableCombo {
            id: cityEdit
            width: parent.width
            editableText: weatherModel.cityName + ", " + weatherModel.countryID
            onEnterPressed: {
                weatherCommon.search(editableText)
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
                checked: weatherCommon.tempScale === WeatherCommon.Celsium
                ButtonGroup.group: scaleGroup
            }
            SettingsRadio {
                id: fahr
                font.pixelSize: 18
                text: "\u00B0F"
                checked: weatherCommon.tempScale === WeatherCommon.Fahrenheit
                ButtonGroup.group: scaleGroup
            }
        }
        SettingsCheck {
            id: systray
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather in menubar")
            checked: weatherCommon.menuBarWeather
        }
        SettingsCheck {
            id: startup
            width: parent.width
            leftPadding: 0
            text: qsTr("Start at login")
            checked: weatherCommon.runAtStartup
        }
        SettingsCheck {
            id: anim
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather animation")
            checked: weatherCommon.showAnimation
        }
        Row {
            width: parent.width
            spacing: 10
            SettingsButton {
                width: parent.width/2 - 5
                text: qsTr("Apply")
                onClicked: {
                    weatherCommon.menuBarWeather = systray.checked
                    weatherCommon.runAtStartup = startup.checked
                    weatherCommon.showAnimation = anim.checked
                    weatherCommon.tempScale = fahr.checked ? WeatherCommon.Fahrenheit : WeatherCommon.Celsium
                    waiwedItem.menuButton.state = "menu"
                    weatherCommon.requestWeatherUpdate()
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
