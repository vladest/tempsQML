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

    ListModel {
        id: citiesModel
    }

    function loadCities() {
        var i = 0
        do {
            var val = settings.value(i,"", "cities");
            if (val === undefined || val === "")
                break;
            i++
            console.log("appending city", val)
            citiesModel.append({ city: val });
        } while(true);
    }

    function saveCities() {
        for (var i = 0; i < citiesModel.count; i++) {
            var city = citiesModel.get(i).city
            if (city !== "")
                settings.setValue(i, city , "cities");
        }
    }

    function addCity(cityName) {
        var found = false
        for (var i = 0; i < citiesModel.count; i++) {
            if (cityName === citiesModel.get(i).city) {
                found = true
                break
            }
        }
        if (found === false) {
            citiesModel.append({ city: cityName });
        }
    }

    Timer {
        id: findTimer
        repeat: false
        interval: searchDelay
        onTriggered: {
            var ind = combo.find(combo.contentItem.text, followSearchFlags)
            fav.checkState = (ind !== -1) ? Qt.Checked : Qt.Unchecked
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
        onCurrentIndexChanged: {
            textField.text = citiesModel.get(currentIndex).city
            weatherCommon.search(textField.text)
        }
        contentItem: TextField {
            id: textField
            padding: 2
            text: combo.currentText
            font: combo.font
            rightPadding: 40
            //color: "#ffffff"
            onTextChanged: {
                findTimer.restart()
            }
            Keys.onReturnPressed: {
                enterPressed()
            }
            background: Item {
                Rectangle {
                    width: parent.width
                    height: 2
                    y: parent.height - 2
                    color: "#999999"
                }
                Button {
                    id: button
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: fav.left
                    width: 20; height: 20;
                    background: PartialImage {
                        width: 20; height: 20;
                        imageOffsetX: -120
                        scale: button.pressed ? 0.80 : 1
                    }
                    onClicked: {
                        weatherCommon.search()
                    }
                }

                CheckBox {
                    id: fav
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    width: 20; height: 20;
                    indicator: PartialImage {
                        width: 20; height: 20;
                        imageOffsetX: fav.checked ? -100 : -80
                    }
                    onCheckedChanged: {
                        if (checked) {
                            addCity(textField.text)
                            //makes sur its selected by ComboBox
                            combo.currentIndex = citiesModel.count - 1
                        } else {
                            citiesModel.remove(combo.currentIndex, 1)
                        }
                    }
                }
            }
        }
        Component.onCompleted: contentItem.width = width - 40
    }
}
