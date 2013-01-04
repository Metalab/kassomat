import QtQuick 2.0

Rectangle{
    property var ps

    id: pay_screen
    visible: (opacity == 0.0)? false : true;
    opacity: (parent.state == "PAY_SCREEN")
    Behavior on opacity{ NumberAnimation { easing.type: Easing.InOutExpo; duration: 1000 } }
    anchors.fill: parent

    color: "#eeeeee"
    border.width: 1
    border.color: "black";
    radius: 10

    anchors.margins: 10

    Text {
        anchors.centerIn: parent
        opacity: parent.opacity
        text: "ZAHLE (mit euros..)"
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            parent.parent.state = "STANDARD_SCREEN"
        }
    }
}
