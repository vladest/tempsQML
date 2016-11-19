import QtQuick 2.5
import QtQuick.Controls 2.2

Item {
    id: root

    property int followSearchFlags: Qt.MatchContains
    property int searchDelay: 400
    property alias interval: findTimer.interval
    property alias textRole: combo.textRole
    property alias currentIndex: combo.currentIndex
    property alias currentText: combo.currentText
    property alias editableText: textField.text
    property alias model: combo.model
    property alias count: combo.count
    signal enterPressed()
    height: combo.height

    Component.onDestruction: {
        for (var i = 1; i < citiesModel.count; i++) {
            var city = citiesModel.get(i).display
            if (city !== "")
                settings.setValue(i, city , "cities");
        }
    }

    ListModel {
        id: citiesModel
    }

    function loadSearchStrings() {
        var i = 1
        do {
            var val = settings.value(i,"", "cities");
            if (val === undefined || val === "")
                break;
            i++
            citiesModel.addString(val)
        } while(true);
    }

    Timer {
        id: findTimer
        repeat: false
        interval: searchDelay
        onTriggered: {
            var ind = combo.find(combo.contentItem.text, followSearchFlags)
            if (ind !== -1)
                combo.currentIndex = ind
        }
    }

    ComboBox {
        id: combo
        width: root.width
        model: citiesModel
        font.pixelSize: 16
        padding: 2
        rightPadding: 2
        flat: true
        contentItem: TextField {
            id: textField
            padding: 2
            text: combo.displayText
            font: combo.font
            //color: "#ffffff"
            onTextChanged: {
                findTimer.restart()
            }
            Keys.onReturnPressed: {
                enterPressed()
            }
            background: Rectangle {
                width: parent.width
                height: 2
                y: parent.height - 2
                color: "#999999"
            }
        }
        Component.onCompleted: contentItem.width = width - 40
    }
}
