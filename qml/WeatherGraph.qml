import QtQuick 2.7
import QtCharts 2.0
import QtQml.Models 2.2
import weathermodel 1.0

Item {
    id: weatherChart
    property int graphPoints: 0

    //delegate model used as a proxy to extract filtered data
    DelegateModel {
        id: visualModel
        model: filteredWeatherModel
        delegate: Item {}
        onCountChanged: {
            //console.log("filtered count", count)
            if (count > 0) {
                tempsSeries.removePoints(0, graphPoints)
                valueAxisX.max = Math.max(count - 1, 1)
                var min = weatherCommon.roundup(weatherCommon.convertToCurrentScale(model.data(visualModel.modelIndex(0), 258)))
                tempsSeries.append(0, min)
                var max = min
                if (count === 1) {
                    //simulating line
                    tempsSeries.append(1, min)
                } else {

                    for (var i = 1; i < count; i++) {
                        var val = weatherCommon.roundup(weatherCommon.convertToCurrentScale(model.data(visualModel.modelIndex(i), 258)))
                        tempsSeries.append(i, val)
                        if (val > max)
                            max = val
                        else if (val < min)
                            min = val
                        //console.log(val)
                    }
                }

                graphPoints = Math.max(count, 2)

                valueAxisY.min = min - 1
                valueAxisY.max = max + 1
            }
        }
    }

    ChartView {
        id: chartView

        anchors.fill: parent
        legend.visible: false
        margins.bottom: 0
        margins.top: 0
        margins.left: 0
        margins.right: 0
        backgroundColor: "transparent"
        antialiasing: true
        clip: false

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
            width: 4*appscale
            style: Qt.SolidLine
            capStyle: Qt.RoundCap
            pointLabelsClipping: false
            pointLabelsVisible: true
            pointLabelsColor: "#999999"
            pointLabelsFont.pixelSize: 12*appscale
            pointLabelsFormat: weatherCommon.tempScale === WeatherCommon.Celsium ? "@yPoint \u00B0C" : "@yPoint \u00B0F"
            axisX: valueAxisX
            axisY: valueAxisY
            color: weatherCommon.backgroundColor
            pointsVisible: true
            //useOpenGL: true

            function updateWeather(point) {
                var _index = Math.round(point.x)
                //index safety
                if (_index >= visualModel.model.count && visualModel.model.count > 0)
                    _index = visualModel.model.count - 1
                weatherLegend.x = chartView.width/graphPoints * point.x - weatherLegend.width / 2
                weatherLegend._temp = weatherCommon.roundup(point.y)
                weatherLegend._time = Qt.formatDateTime(visualModel.model.data(visualModel.modelIndex(_index), 257), "ddd hh:mm")
                weatherLegend._iconid = visualModel.model.data(visualModel.modelIndex(_index), 268)
            }

            onPressed: {
                weatherLegend.visible = true
                updateWeather(point)
            }

            //only for systems with mouse
            onHovered: {
                weatherLegend.visible = state

                if (state) {
                    updateWeather(point)
                }
            }
        }
    }

    MenuClose {
        state: "back"
        color: weatherCommon.backgroundColor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10*appscale
        anchors.topMargin: -20*appscale
        height: 20*appscale
        width: 20*appscale

        MouseArea {
            anchors.fill: parent
            onClicked: {
                weatherLegend.visible = false
                subdetails.showdetailsindex = -1
            }
        }
    }

    Item {
        id: weatherLegend
        visible: false
        property int _temp: 0
        property string _iconid: ""
        property string _time: ""
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5*appscale
        height: 50*appscale
        width: 60*appscale
        Column {
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                ColoredImage {
                    source: weatherLegend._iconid !== "" ? "images/icons/" + weatherLegend._iconid + ".svg" : ""
                    width: 32*appscale
                    height: 32*appscale
                    overlayColor: "#999999"
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: weatherLegend._temp + "\u00B0"+weatherCommon.temperatureScale
                    color: "#999999"
                    font.pixelSize: 18*appscale
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: weatherLegend._time
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#999999"
                font.pixelSize: 12*appscale
            }
        }

    }
}

