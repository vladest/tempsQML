import QtQuick 2.6
import QtQuick.Controls 2.0

CheckBox {
    id: control

    indicator: Item {
        x: control.width - control.rightPadding -  20
        y: parent.height / 2 - height / 2
        width: 20; height: 20;
        clip: true

        Image {
            source: "images/checkboxes@2x.png"
            cache: false
            x: control.checked ? -20 : 0
            y: 0
            sourceSize.height: 20
            sourceSize.width: 140
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: "#999999"
        leftPadding: control.leftPadding
    }
}
