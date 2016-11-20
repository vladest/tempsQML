import QtQuick 2.5

Item {
    property real imageOffsetX: 0
    property real imageOffsetY: 0
    clip: true

    Image {
        source: "images/checkboxes@2x.png"
        cache: false
        x: imageOffsetX
        y: imageOffsetY
        sourceSize.height: 20
        sourceSize.width: 140
    }
}
