import QtQuick 2.0


Rectangle {
    id: changeButton
    property int selected: 0; // anzahl der scheine/muenzen
    property int value: 0; // geldwert

    signal countChanged(int value, int selected)

    width: button_pic.sourceSize.width
    height: button_pic.sourceSize.height
    color: transparent

    clip: true

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                changeButton.selected += 1;
                //if( app.credits >= value )
                changeButton.countChanged(changeButton.value, changeButton.selected);
            }
        }
    }

    Image {
        id: button_pic
        source: changeButton.value + '.png'
    }

    Text {
        text: selected
        color: "red"
    }
}
