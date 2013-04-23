import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    property var money_back: ['5000','2000','1000','500',
        '200','100','50','20','10','5','2','1'];

    id: change_screen_wrapper
    color: "#eeeeee"

    MouseArea{
                anchors.fill: parent;
    }
    
    Rectangle{
        id: change_screen
        anchors.margins: 15
        anchors.fill: parent
        color: "transparent"

        Row{
            id:money_row
            spacing: 5

            Repeater {
                model: money_back

                delegate: ChangeButton {
                    color: "transparent"
                    // modelData === money_back[i]
                    value: modelData
                    onCountChanged: {
                        // javascript code
                        console.log('Die Anzahl von ' + value + ' ist jetzt ' + selected)
                    }
                }
            }
        }

        Button{
            id: fill_up_button
            height: 40
            width: 200

            color: "#444422";
            anchors.top: money_row.bottom

            onButtonClick: {

            }
        }

        Button{
            id: accept_button
            height: 40
            width: 200

            color: "#444422";

            anchors.top: money_row.bottom
            anchors.left: fill_up_button.right

            onButtonClick: {

            }
        }

        Button{
            id: cancel_button
            height: 40
            width: 200

            color: "#444422";
            anchors.top: money_row.bottom
            anchors.left: accept_button.right

            onButtonClick: {
                controller.state = "STANDARD_SCREEN"
            }
        }
    }    

    DropShadow {
            id: windowShadow
            z: change_screen_wrapper.z-1
            anchors.fill: change_screen_wrapper
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 16
            samples: 32
            color: "#aa000000"
            transparentBorder: true
            fast: true
            source: change_screen_wrapper
    }
}


