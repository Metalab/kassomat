import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0

// bei bezahlen bildschirm, moeglichkeit geben zu spenden
// oder rest an ein kickstarter-projekt
// potentieller schotter auf ibutton raufladen wenn schotter gefahr besteht

Rectangle{
    id: app
    width: 1280
    height: 768

    property int f: 0;

    state: controller.state

    states:[
            State {
                name: "STANDARD_SCREEN"
            },
            State {
                name: "PAY_SCREEN"
            },
            State {
                name: "CHANGE_SCREEN"
            }
        ]



    Rectangle{
        id: mainframe

        anchors.left: parent.left
        width: 1050
        height: parent.height

        StandardScreen{
            id: screeen
            visible: true
            //enabled: (controller.state == "STANDARD_SCREEN")
            anchors.fill: parent            
        }

        FastBlur{
                id: inactive
                anchors.fill: screeen
                source: screeen
                visible: (radius == 0)? false : true;
                radius: (controller.state == "STANDARD_SCREEN")? 0 : 32;
                Behavior on radius{
                    NumberAnimation {
                        easing.type: Easing.Linear
                        duration:250
                    }
                }

                MouseArea{
                    enabled: (controller.state == "STANDARD_SCREEN")? false : true;
                    anchors.fill: parent;
                    onClicked:{
                        controller.state = "STANDARD_SCREEN";
                    }
                }
        }

        ChangeScreen{
            visible: (opacity == 0.0)? false : true;
            opacity: (controller.state == "CHANGE_SCREEN")
            anchors.centerIn: parent

            enabled:true

            width: 900
            height: 300

            Behavior on opacity{
                NumberAnimation {
                    easing.type: Easing.Linear
                    duration:250
                }
            }

        }

        PayScreen{
            visible: (opacity == 0.0)? false : true;
            opacity: (controller.state == "PAY_SCREEN")
            anchors.centerIn: parent

            enabled:true

            width: 900
            height: 200

            Behavior on opacity{
                NumberAnimation {
                    easing.type: Easing.Linear
                    duration:250
                }
            }

        }
    }

    Rectangle{
        id: actions_col
        color: "black"

        anchors.right: parent.right
        width: 230
        height: parent.height

        ActionBlock{
                anchors.fill: parent
                anchors.margins: 10
        }

    }

}


