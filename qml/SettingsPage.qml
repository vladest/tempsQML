import QtQuick 2.5
import QtQuick.Controls 2.0

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

            SettingsRadio {
                font.pixelSize: 18
                text: "\u00B0C"
            }
            SettingsRadio {
                font.pixelSize: 18
                text: "\u00B0F"
            }
        }
        SettingsCheck {
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather in menubar")
        }
        SettingsCheck {
            width: parent.width
            leftPadding: 0
            text: qsTr("Start at login")
        }
        SettingsCheck {
            width: parent.width
            leftPadding: 0
            text: qsTr("Show weather animation")
        }
        Row {
            width: parent.width
            spacing: 10
            SettingsButton {
                width: parent.width/2 - 5
                text: qsTr("Apply")
            }
            SettingsButton {
                width: parent.width/2 - 5
                text: qsTr("Quit")
            }
        }
    }
}
