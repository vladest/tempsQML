import QtQuick 2.7
import QtQuick.Controls 2.2

CheckBox {
    id: control
    font.pixelSize: appRoot.height/32

    indicator: PartialImage {
            x: control.leftPadding
            y: parent.height / 2 - height / 2
            scale: appscale
            width: 20;
            height: 20;
            imageOffsetX: control.checked ? -20 : 0
        }


    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#999999"
        leftPadding: control.leftPadding + indicator.width + 10
    }
}
