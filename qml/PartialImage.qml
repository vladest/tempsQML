import QtQuick 2.7
import QtGraphicalEffects 1.0
Item {
    property real imageOffsetX: 0
    property real imageOffsetY: 0
    clip: true

    Image {
        id: img
        source: "images/checkboxes@2x.png"
        cache: false
        x: imageOffsetX
        y: imageOffsetY
        sourceSize.height: 20
        sourceSize.width: 140
    }
}
