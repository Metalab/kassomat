import QtQuick 2.0


Rectangle {
    property alias img_id: button_pic
    property var action
    property int selected:0;
    property int value:0;

    width: button_pic.sourceSize.width
    height: button_pic.sourceSize.height
    color: transparent

    clip: true

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button == Qt.LeftButton){
                //if( app.credits >= value )
                selected += 1;
                action[0]();
            }
        }
    }

    Image {
        id: button_pic
    }

    Text {
        text: selected
        color: "red"
    }
}
