import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "."

ApplicationWindow {
    id: appRoot
    visible: true

    width: 280
    height: 480
    title: qsTr("tempsQML")
    property alias backgr: mainView.background
    property alias mainView: mainView
    property real appscale: appRoot.height/480

    //store items, pushed to StackView
    property Item itemSettings: null
    property Item itemMain: null

    signal settingsClicked

    MenuClose {
        id: menuButton
        height: appRoot.height/24
        width: height
        z: 1000
        onStateChanged: {
            settingsClicked()
        }

        anchors { left: parent.left; top: parent.top;
            leftMargin: appRoot.width/28;
            topMargin: appRoot.height/28;
        }

        MouseArea {
            anchors.fill: parent
            onClicked: menuButton.state = (menuButton.state == "menu" ? "back" : "menu")
        }
    }

    StackView {
        id: mainView
        padding: 0
        anchors.fill: parent
        initialItem: mainComponent
        popEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 800 }
        }
        popExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 800 }
        }
        pushEnter: Transition {
            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 800 }
        }
        pushExit: Transition {
            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 800 }
        }

        background: WaiwedItem {
            id: waiwedItem
            Connections {
                target: menuButton
                onStateChanged: {
                    if (menuButton.state === "menu") {
                        mainView.pop()
                    } else {
                        itemSettings = mainView.push(settingsComponent)
                    }
                }
            }
        }
    }

    Component {
        id: settingsComponent
        SettingsPage {}
    }

    Component {
        id: mainComponent
        MainPage {}
    }

    Component {
        id: mapComponent
        WeatherMap {}
    }

    Component.onCompleted: {
        browserCoordinate.requestCoordinates()
    }
}
