import QtQuick 2.0

import QtQuick 2.0

Rectangle{
    id: standard_screen
    visible: (opacity == 0.0)? false : true;
    Behavior on opacity{ NumberAnimation { easing.type: Easing.InQuad; duration:500; } }

    anchors.fill: parent

    Row{
        y:0

        spacing: 18
        anchors.fill: parent
        anchors.margins: 10

        Column{
            id: buystuff_col
            width:425

            spacing:5

            FridgeBlock{
                width: parent.width
            }

            JobsBlock{
                width: parent.width
            }

        }

        Column{
            id: promote_col
            width: 550
            spacing: 10

            ProjectBlock{
                width: parent.width
            }

            CommunityBlock{
                width: parent.width
            }

        }

        Rectangle{
            id: actions_col
            color: "black"
            width: 230
            height: parent.height

            ActionBlock{

            }

        }
    }

/*

    //property var actions
    //property var ps

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
    }*/
}

