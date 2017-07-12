import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
    id: root

    property int followSearchFlags: Qt.MatchContains
    property int searchDelay: 100
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
        settings.setValue("", "", "cities"); // clear group
        for (var i = 0; i < citiesModel.count; i++) {
            var city = citiesModel.get(i).city
            if (city !== "") {
                settings.setValue(i, city , "cities");
            }
        }
    }

    function searchCity(cityName) {
        for (var i = 0; i < citiesModel.count; i++) {
            if (cityName === citiesModel.get(i).city) {
                return true
            }
        }
        return false
    }

    function addCity(cityName) {
        if (searchCity(cityName) === false) {
            citiesModel.append({ city: cityName });
        }
    }

    Timer {
        id: findTimer
        repeat: false
        interval: searchDelay
        onTriggered: {
            fav.checkState = searchCity(textField.text) ? Qt.Checked : Qt.Unchecked
        }
    }

    ComboBox {
        id: combo
        width: root.width
        model: citiesModel
        height: 24*appscale
        font.pixelSize: appRoot.height / 30
        padding: 2*appscale
        rightPadding: 2*appscale
        background: Item {
            width: parent.width
            height: parent.height

            Row {
                id: backgrrow
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: combo.indicator.width - combo.padding
                spacing: 0
                Button {
                    id: button
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height
                    width: height
                    background: PartialImage {
                        anchors.centerIn: parent
                        width: 20; height: 20;
                        imageOffsetX: -120
                        scale: button.pressed ? 0.80*appscale : appscale
                    }
                    onClicked: {
                        weatherCommon.search()
                    }
                }

                CheckBox {
                    id: fav
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height
                    width: height
                    indicator: PartialImage {
                        width: 20; height: 20;
                        anchors.centerIn: parent
                        scale: appscale
                        imageOffsetX: fav.checked ? -100 : -80
                    }
                    onCheckedChanged: {
                        if (checked) {
                            addCity(textField.text)
                            //makes sur its selected by ComboBox
                            combo.currentIndex = citiesModel.count - 1
                        } else {
                            console.log("remove index", combo.currentIndex)
                            citiesModel.remove(combo.currentIndex, 1)
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 2*appscale
                y: parent.height + 2*appscale
                color: "#999999"
            }
        }
        flat: true
        onCurrentIndexChanged: {
            textField.text = citiesModel.get(currentIndex).city
            weatherCommon.search(textField.text)
        }
        contentItem: TextField {
            id: textField
            padding: 2*appscale
            text: combo.currentText
            font: combo.font
            //rightPadding: appRoot.height / 12
            //color: "#ffffff"
            onTextChanged: {
                findTimer.restart()
            }
            Keys.onReturnPressed: {
                enterPressed()
            }
            background: Item {}
        }
        Component.onCompleted:  {
            combo.contentItem.width = Qt.binding(function() { return combo.width - backgrrow.width - indicator.width})
        }
    }

}

