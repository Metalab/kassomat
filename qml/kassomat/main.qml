import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0
import QtQuick.Window 2.1

// bei bezahlen bildschirm, moeglichkeit geben zu spenden
// oder rest an ein kickstarter-projekt
// potentieller schotter auf ibutton raufladen wenn schotter gefahr besteht

Window{
    id: app
    width: 1280
    height: 768
    title: "Kassomat"
    visible:true

    onClosing: database.dbTestCleanup();
    Component.onCompleted: console.log('dont worry');

    Rectangle{

        anchors.fill: parent
        property int f: 0;

        focus:true

        Keys.onLeftPressed: {
            controller.credit += 50;
            console.log('50 cent, yo!')
        }
        Keys.onUpPressed: {
            controller.credit += 100;
            console.log('1 euro, yo!')
        }
        Keys.onRightPressed: {
            controller.credit += 20;
            console.log('20 cent, yo!')
        }
        Keys.onDownPressed: {
            controller.credit += 10;
            console.log('10 cent, yo!')
        }
        Keys.onEnterPressed: {
            controller.credit = 0;
            console.log('10 cent, yo!')
        }

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
            },
            State {
                name: "PROJECT_SCREEN"
            }
        ]


        Rectangle{
            id: mainframe

            anchors.left: parent.left
            width: 1050
            height: parent.height

            // inactive blur effect --------------------------------------------

            MouseArea{
                enabled: (controller.state == "STANDARD_SCREEN")? false : true;
                anchors.fill: parent;
                onClicked:{
                    controller.state = "STANDARD_SCREEN";
                }
            }

            // all the screens ----------------------------------

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
            }

            ChangeScreen{
                visible: (opacity == 0.0)? false : true;
                opacity: (controller.state == "CHANGE_SCREEN")
                anchors.centerIn: parent

                width: 900
                height: 400

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

                width: 900
                height: 200

                Behavior on opacity{
                    NumberAnimation {
                        easing.type: Easing.Linear
                        duration:250
                    }
                }
            }

            ProjectScreen{
                visible: (opacity == 0.0)? false : true;
                opacity: (controller.state == "PROJECT_SCREEN")
                anchors.fill: parent

                width: parent.width
                height: parent.height

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

}


