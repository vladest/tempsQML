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
                target: waiwedItem.menuButton
                onStateChanged: {
                    if (waiwedItem.menuButton.state === "menu") {
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
