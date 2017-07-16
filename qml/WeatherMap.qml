import QtQuick 2.7
import QtLocation 5.9
import QtPositioning 5.8
import QtQuick.Controls 2.2

Item {
    property var centerCoordinate: browserCoordinate.browserCoordinate
    Map {
        id: baseMap
        anchors.fill: parent
        center: weatherMap.center
        zoomLevel: weatherMap.zoomLevel
        activeMapType: supportedMapTypes[0]

        plugin: Plugin {
            allowExperimental: true
            name: "osm"
            locales: "en_EN"
        }
    }
    Map {
        id: weatherMap
        anchors.fill: parent
        z: baseMap.z + 1
        center: centerCoordinate
        gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.FlickGesture | MapGestureArea.PinchGesture
        gesture.flickDeceleration: 3000
        gesture.enabled: true
        focus: true
        color: "transparent"
        activeMapType: supportedMapTypes[0]

        plugin: Plugin {
            allowExperimental: true
            name: "openweathermaps"
            locales: "en_EN"
            PluginParameter {
                name: "openweathermaps.appkey"
                value: "483240f16c1aa9e5feab1fa41c9d273a"
            }
        }
    }
    ListModel {
        id: layersListModel
    }

    Row {
        width: parent.width
        anchors { left: parent.left; top: parent.top; margins: 10; right: parent.right }
        z: 1000
        spacing: 10
        MenuClose {
            state: "back"
            height: 30*appscale
            width: 30*appscale

            MouseArea {
                anchors.fill: parent
                onClicked: mainView.pop()
            }
        }
        ComboBox {
            model: layersListModel
            width: parent.width - 50
            currentIndex: 0
            onCurrentIndexChanged: weatherMap.activeMapType = weatherMap.supportedMapTypes[currentIndex]
        }
    }
    Component.onCompleted: {
        for (var i = 0; i < weatherMap.supportedMapTypes.length; i++) {
            layersListModel.append({display: weatherMap.supportedMapTypes[i].name})
        }
    }
}
