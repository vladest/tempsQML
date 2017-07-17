import QtQuick 2.7
import QtQuick.Controls 2.2

Item {
    id: root

    MouseArea {
        anchors.fill: parent
        onClicked: subdetails.showdetailsindex = -1
    }

    Button {
        height: appRoot.height/24
        width: height
        anchors { top: parent.top; topMargin: appRoot.height/24+appRoot.height/28*2; left: parent.left; leftMargin: parent.width / 28; }
        background: ColoredImage {
            overlayColor: "white"
            source: "images/earth.svg"
        }
        onClicked: {
            mainView.push(mapComponent)
        }
    }
    ColoredImage {
        width: appRoot.height / 4.8
        height: width
        overlayColor: "white"
        anchors { top: parent.top; topMargin: parent.height * 0.14; right: parent.right; rightMargin: parent.width * 0.04; }
        source: weatherModel.currentWeather.weather_codition_icon_id === "" ?
                    "images/icons/IconTemplate@02d.png" :
                    "images/icons/" + weatherModel.currentWeather.weather_codition_icon_id + ".svg"
    }
    Column {
        anchors.top: parent.top
        anchors.topMargin: appRoot.height / 4
        anchors.horizontalCenter: parent.horizontalCenter
        height: appRoot.height / 4.8

        Row {
            id: degrow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: degree.paintedWidth/2
            spacing: 0
            Text {
                font.pixelSize: root.height / 10
                color: "white"
                text: weatherCommon.roundup(weatherCommon.convertToCurrentScale(weatherModel.currentWeather.temp))
            }
            Text {
                id: degree
                font.pixelSize: root.height / 10
                color: "white"
                text: "\u00B0"
            }

        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            //opacity: 0.5
            width: degrow.width
            height: 2
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: root.height / 26
            color: "white"
            opacity: 0.5
            text: weatherModel.currentWeather.weather_codition_description
        }
    }
    Item {
        id: details
        anchors.bottom: parent.bottom
        width: parent.width
        height: appRoot.height / 3
        Row {
            id: row
            width: parent.width
            spacing: 0
            anchors.top: parent.top

            Text {
                width: parent.width/2
                font.pixelSize: appRoot.height / 40
                color: "#999999"
                text: Qt.formatDate(weatherModel.currentWeather.timestamp, "dddd, MMM d")
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                width: parent.width/2
                font.pixelSize: appRoot.height / 40
                color: "#999999"
                text: weatherModel.cityName.toLowerCase() + ", " + weatherModel.countryID.toLowerCase()
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Item {
            id: subdetails
            anchors.bottom: parent.bottom
            width: parent.width
            anchors.top: row.bottom
            property int showdetailsindex: -1
            ListView {
                id: view
                model: weatherDailyModel
                visible: subdetails.showdetailsindex === -1
                anchors.fill: parent
                anchors.margins: 10
                clip: false
                interactive: true
                focus: true
                orientation: ListView.Horizontal
                highlightFollowsCurrentItem: true

                delegate: WeatherDelegate {
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (index >= 0 && index < weatherModel.daysNumber) {
                                subdetails.showdetailsindex = index
                                filteredWeatherModel.setWeatherDateIndex(index)
                            }
                        }
                    }
                }
            }
            WeatherGraph {
                anchors.fill: parent
                visible: subdetails.showdetailsindex !== -1
            }
        }
    }
}

