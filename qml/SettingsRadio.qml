import QtQuick 2.7
import QtQuick.Controls 2.2

RadioButton {
    id: control

    indicator: PartialImage {
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        scale: appscale
        width: 20; height: 20;
        imageOffsetX: control.checked ? -60 : -40
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#000000"
        leftPadding: control.indicator.width*appscale + control.spacing
    }
}
