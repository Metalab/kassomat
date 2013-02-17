import QtQuick 2.0

Rectangle{
    property var actions
    property var ps

    id: standard_screen
    visible: (opacity == 0.0)? false : true;
    Behavior on opacity{ NumberAnimation { easing.type: Easing.InQuad; duration:500; } }
    anchors.fill: parent

    Button {
        y: 20
        anchors.horizontalCenter: parent.horizontalCenter
        buttonWidth: 100
        label: "Donate"
        onButtonClick: {
        }
    }

    BubbleButton{
        id: pay_button
        height: 400
        width: 200

//        ps:ps

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        action: [function(){
            parent.parent.state = "PAY_SCREEN"
        }]

        img_id.source: "mate.png";
        img_id.transform: [
            Rotation { angle: 20 },
            Translate { y: 0; x: 130 }
        ]
    }

    BubbleButton{
        id: change_button
        height: 256
        width: 256

        anchors.left: pay_button.right
        anchors.bottom: pay_button.bottom
        anchors.leftMargin: parent.width - pay_button.width - 20 - width - 20

        action: [function(){
            parent.parent.state = "CHANGE_SCREEN"
        }]

        img_id.source: "change.png";
    }
}
