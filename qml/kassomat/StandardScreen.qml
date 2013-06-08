import QtQuick 2.0

import QtQuick 2.0

Rectangle{
    id: standard_screen

    Row{
        y:0

        spacing: 18
        anchors.fill: parent
        anchors.margins: 10

        Column{
            id: buystuff_col
            width:555

            spacing:15

            FridgeBlock{
                width: parent.width
            }

            JobsBlock{
                width: parent.width
            }

        }

//        Column{
//            id: promote_col
//            width: 550
//            spacing: 10

        ProjectBlock{
            width: 420
            height: parent.height
        }

//            CommunityBlock{
//                width: parent.width
//            }

//        }
    }
}

