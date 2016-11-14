import QtQuick 2.5

Item {
  id: mainRect

  property real radius: 3

  Rectangle {
    id: bar1
    x: 0
    y: parent.height * .25
    width: parent.width
    height: parent.height * .15
    radius: parent.radius
    antialiasing: true
  }

  Rectangle {
    id: bar2
    x: 0
    y: parent.height * .5
    width: parent.width
    height: parent.height * .15
    radius: parent.radius
    antialiasing: true
  }

  Rectangle {
    id: bar3
    x: 0
    y: parent.height * .75
    width: parent.width
    height: parent.height * .15
    radius: parent.radius
    antialiasing: true
  }

  property int animationDuration: 350

  state: "menu"
  states: [
    State {
      name: "menu"
    },

    State {
      name: "back"
      PropertyChanges { target: mainRect; rotation: 180 }
      PropertyChanges { target: bar1; rotation: 45; width: mainRect.width * .6; x: mainRect.width * .48; y: mainRect.height * .33 - bar1.height }
      PropertyChanges { target: bar2; width: mainRect.width * .8; x: mainRect.width * .2; y: mainRect.height * .5 - bar2.height }
      PropertyChanges { target: bar3; rotation: -45; width: mainRect.width * .6; x: mainRect.width * .48; y: mainRect.height * .67 - bar3.height }
    }
  ]

  transitions: [
    Transition {
      RotationAnimation { target: mainRect; direction: RotationAnimation.Clockwise; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar1; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar2; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
      PropertyAnimation { target: bar3; properties: "rotation, width, x, y"; duration: animationDuration; easing.type: Easing.InOutQuad }
    }
  ]
}
