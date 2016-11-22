import QtQuick 2.5
import QtCharts 2.2
import QtQml.Models 2.2

Item {
    id: root

    property int graphPoints: 0
    Image {
        width: 80
        height: 80
        anchors { top: parent.top; topMargin: parent.height * 0.14; left: parent.left; leftMargin: parent.width * 0.08; }
        source: weatherModel.currentWeather.weather_codition_icon_id === "" ?
                    "images/icons/IconTemplate@02d.png" :
                    "images/icons/" + weatherModel.currentWeather.weather_codition_icon_id + ".svg"
    }
    Column {
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.3 - 30
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: degree.paintedWidth/2
            spacing: 0
            Text {
                font.pixelSize: 48
                color: "white"
                text: weatherCommon.roundup(weatherCommon.convertToCurrentScale(weatherModel.currentWeather.temp))
            }
            Text {
                id: degree
                font.pixelSize: 48
                color: "white"
                text: "\u00B0"
            }

        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            //opacity: 0.5
            width: 24
            height: 2
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 18
            color: "white"
            opacity: 0.5
            text: weatherModel.currentWeather.weather_codition_description
        }
    }
    Item {
        id: details
        anchors.bottom: parent.bottom
        width: parent.width
        height: 160
        Row {
            id: row
            width: parent.width
            spacing: 0
            anchors.top: parent.top

            Text {
                width: parent.width/2
                font.pixelSize: 12
                color: "#999999"
                text: Qt.formatDate(weatherModel.currentWeather.timestamp, "dddd, MMM d")
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                width: parent.width/2
                font.pixelSize: 12
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
                clip: true
                interactive: true
                focus: true
                orientation: ListView.Horizontal
                highlightFollowsCurrentItem: true

                delegate: WeatherDelegate {
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            subdetails.showdetailsindex = index
                            filteredWeatherModel.setWeatherDateIndex(index)
                        }
                    }
                }
            }

            DelegateModel {
                id: visualModel
                model: filteredWeatherModel
                delegate: XYPoint {
                    x: index
                    y: weatherCommon.roundup(weatherCommon.convertToCurrentScale(temp))
                }
                onCountChanged: {
                    console.log("filtered count", count)
                    if (count > 0) {
                        console.log("points", graphPoints)
                        tempsSeries.removePoints(0, graphPoints - 1)
                        valueAxisX.max = count - 1
                        var min = weatherCommon.roundup(weatherCommon.convertToCurrentScale(model.data(visualModel.modelIndex(0), 258)))
                        tempsSeries.append(0, min)
                        var max = min
                        for (var i = 1; i < count; i++) {
                            var val = weatherCommon.roundup(weatherCommon.convertToCurrentScale(model.data(visualModel.modelIndex(i), 258)))
                            tempsSeries.append(i, val)
                            if (val > max)
                                max = val
                            else if (val < min)
                                min = val
                            console.log(val)
                        }
                        graphPoints = count

                        valueAxisY.min = min - 1
                        valueAxisY.max = max + 1
                    }
                }
            }

            ChartView {
                visible: subdetails.showdetailsindex !== -1
                anchors.fill: parent
                legend.visible: false
                margins.bottom: 0
                margins.top: 0
                margins.left: 0
                margins.right: 0
                backgroundColor: "transparent"
                antialiasing: true

                ValueAxis {
                    visible: false
                    id: valueAxisX
                    min: 0
                    max: 1
                }

                ValueAxis{
                    visible: false
                    id: valueAxisY
                    min: 0
                    max: 5
                }

                SplineSeries {
                    id: tempsSeries
                    width: 4
                    style: Qt.SolidLine
                    capStyle: Qt.RoundCap
                    axisX: valueAxisX
                    axisY: valueAxisY
                    color: weatherCommon.backgroundColor
                    pointsVisible: true
                    useOpenGL: true

                    onHovered: {
                        console.log(point, state)
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        subdetails.showdetailsindex = -1
                    }
                }

            }
        }
    }
}

