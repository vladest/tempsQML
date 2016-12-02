import QtQuick 2.6
import QtQuick.Controls 2.0

Button {
    id: control

    font.pixelSize: appRoot.height/32

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: hovered ? "#FFFFFF" : "#000000"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        opacity: enabled ? 1 : 0.3
        color: hovered ? "#80BBB2" : "#FFFFFF"
        border.width: 1
        border.color: "#CCCCCC"
    }
}
