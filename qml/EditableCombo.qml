import QtQuick 2.7
import QtQuick.Controls 2.2

ComboBox {
    id: combo

    property int searchDelay: 1000
    property alias interval: findTimer.interval
    signal enterPressed()

    model: citiesModel
    height: 24*appscale
    font.pixelSize: height / 2
    padding: 2*appscale
    rightPadding: 2*appscale

    ListModel {
        id: citiesModel
    }

    Connections {
        target: weatherModel
        onCurrentWeatherChanged: {
            //console.log("current weather changed", weatherModel.cityName, weatherModel.countryID)
            if (weatherModel.cityName !== "" && weatherModel.countryID !== "") {
                combo.editText = weatherModel.cityName + ", " + weatherModel.countryID
            }
        }
    }

    function loadCities() {
        citiesModel.clear()
        var i = 0

        do {
            var val = settings.value(i,"", "cities");
            if (val === undefined || val === "")
                break;
            i++
            console.log("appending city", val)
            addCity(val);
        } while(true);

        var _city = weatherModel.cityName + ", " + weatherModel.countryID;
        if (_city.length > 2) {
            if (citiesModel.count == 0) {
                //nothing gets loaded
                citiesModel.append({ city:  _city});
                currentIndex = 0;
            } else {
                currentIndex = searchCity(_city)
            }
        }
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
        if (cityName.length <= 2) {
            return -1; //emty or just ", "
        }
        for (var i = 0; i < citiesModel.count; i++) {
            //console.log("comparing", cityName.toUpperCase(), citiesModel.get(i).city.toUpperCase(), i)
            if (cityName.toUpperCase() === citiesModel.get(i).city.toUpperCase()) {
                return i;
            }
        }
        return -1;
    }

    function addCity(cityName) {
        if (searchCity(cityName) === -1) {
            citiesModel.append({ city: cityName });
        }
    }

    function queryCityWeather(city) {
        combo.contentItem.placeholderText = qsTr("Querying...")
        weatherCommon.search(city)
    }

    Timer {
        id: findTimer
        repeat: false
        interval: searchDelay
        onTriggered: {
            queryCityWeather(combo.editText)
            var ind = combo.find(combo.editText, Qt.MatchContains)//searchCity(combo.currentText);
            fav.checkState = ind >= 0 ? Qt.Checked : Qt.Unchecked;
        }
    }

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
                        addCity(combo.editText)
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
    editable: true
    onCurrentIndexChanged: {
        //console.log("current index", currentIndex)
        if (currentIndex >= 0) {
            combo.editText = citiesModel.get(currentIndex).city
            queryCityWeather(citiesModel.get(currentIndex).city)
        }
    }

    Connections {
        target: contentItem
        onTextEdited: {
            //console.log("text edited")
            if (combo.editText !== "") {
                findTimer.restart()
            } else {
                findTimer.stop()
            }
        }
    }

//    onEditTextChanged: {
//        console.log("edit text [" + editText + "] [" + currentText + "] [" + displayText + "]")
//    }

    Component.onCompleted:  {
        combo.contentItem.width = Qt.binding(function() { return combo.width - backgrrow.width -
                                                          indicator.width})
        if (weatherModel.cityName !== "" && weatherModel.countryID !== "") {
            combo.editText = weatherModel.cityName + ", " + weatherModel.countryID
        }
    }
}
