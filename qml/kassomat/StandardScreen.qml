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

            spacing:15

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
}

