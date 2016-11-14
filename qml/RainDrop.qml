import QtQuick 2.5

Rectangle {
    gradient: Gradient {
        GradientStop { position: 0.0; color:  Qt.rgba(13, 52, 58, 1) }
        GradientStop { position: 1.0; color: Qt.rgba(255, 255, 255, .6) }
    }
    color: Qt.rgba(255, 255, 255, 0.2);
    width: 1
    height: 50
}
