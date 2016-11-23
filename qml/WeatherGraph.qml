import QtQuick 2.5
import QtCharts 2.2
import QtQml.Models 2.2

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
                    //console.log(val)
                }
                graphPoints = count

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
                weatherLegend.visible = state

                if (state) {
                    var _index = Math.round(point.x)
                    weatherLegend.x = chartView.width/graphPoints * point.x - weatherLegend.width / 2
                    weatherLegend._temp = weatherCommon.roundup(point.y)
                    weatherLegend._time = Qt.formatDateTime(visualModel.model.data(visualModel.modelIndex(_index), 257), "ddd hh:mm")
                    weatherLegend._iconid = visualModel.model.data(visualModel.modelIndex(_index), 268)
                }

                //console.log(point, state)
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
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
        anchors.bottomMargin: 5
        height: 50
        width: 60
        Column {
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                ColoredImage {
                    source: weatherLegend._iconid !== "" ? "images/icons/" + weatherLegend._iconid + ".svg" : ""
                    width: 32
                    height: 32
                    overlayColor: "#999999"
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: weatherLegend._temp + "\u00B0C"
                    color: "#999999"
                    font.pixelSize: 18
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: weatherLegend._time
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter//"mon, 14:00"
                color: "#999999"
                font.pixelSize: 12
            }
        }

    }
}

