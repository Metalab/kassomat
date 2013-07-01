import QtQuick 2.0
import QtGraphicalEffects 1.0

Column{
    spacing: 35

    Title{
        id: metafridge
        label: "METAFRIDGE"
    }

    Button{
        id: mate_button
        buttonWidth:parent.width
        buttonHeight:80
        buttonColor:"#eeeeee"

        label: "Mate/Beer 2€"
        labelSize: 40
        position: 100

        clip: false

        Image{
                width: sourceSize.width/2.5
                height: sourceSize.height/2.5
                anchors.verticalCenter: parent.verticalCenter
                transform: Rotation{
                    origin {
                          x: 60
                          y: parent.width/2
                    }

                    axis {x: 0; y: 0; z: 1}
                    angle: 25
                }
                source: "./mate.png"
        }

        onButtonClick: {
            controller.debt = 200;
            controller.state = "PAY_SCREEN";
        }
    }

    Button{
        buttonWidth:parent.width
        buttonHeight:80
        buttonColor:"#eeeeee"

        label: "Limo 1,50€"
        labelSize: 40
        position: 100

        onButtonClick: {
            controller.debt = 150;
            controller.state = 'PAY_SCREEN';
        }
    }

    Row{
        width:parent.width
        height:80

        spacing: 5

        Button{
            buttonHeight:parent.height
            buttonWidth: (parent.width/2)-2.5
            buttonColor: "#eeeeee"

            label: "Mineral 1€"
            labelSize: 32

            onButtonClick: {
                controller.debt = 100
                controller.state = 'PAY_SCREEN'
            }
        }

        Button{
            height:parent.height
            width:(parent.width/2)-2.5
            buttonColor: "#eeeeee"

            label: "Kaffee 0,50€"
            labelSize: 32

            onButtonClick: {
                controller.debt = 50
                controller.state = 'PAY_SCREEN'
            }
        }
    }
}
