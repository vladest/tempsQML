import QtQuick 2.5
import QtLocation 5.5
import QtPositioning 5.5

Item {
    property var centerCoordinate: browserCoordinate.browserCoordinate
    Map {
        id: baseMap
        anchors.fill: parent
        center: centerCoordinate
        zoomLevel: weatherMap.zoomLevel

        plugin: Plugin {
            id: herePlugin
            allowExperimental: true
            name: "here"
            locales: "en_EN"
            PluginParameter {
                name: "here.app_id"
                value: ""
            }
            PluginParameter {
                name: "here.token"
                value: ""
            }
            PluginParameter {
                name: "here.proxy"
                value: "system"
            }
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
        activeMapType: supportedMapTypes[2]

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

}
