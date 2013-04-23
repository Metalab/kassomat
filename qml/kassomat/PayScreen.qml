import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle{
    id: pay_screen_wrapper

    MouseArea{
                anchors.fill: parent;
    }

    property int debit: 0

    //Row{
        Column{
            anchors.fill: parent
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                text: "WTF?! GIMME MONEHY!"

                color: "black"
                font.pointSize: 40
            }

            Button{
                height:80
                width:200
                buttonColor: "#eeeeee"

                label: "Yah, sure"
                labelSize: 40

                anchors.horizontalCenter: parent.horizontalCenter

                onButtonClick: {
                    controller.credit -= debit;
                    controller.state = "STANDARD_SCREEN";
                }
            }

        //}

        /*Image{
            id: bitcoin_image
            height: 256
            width: 256
            //source: "image://myimageprovider/qrcode.png"
        }*/
    }

    DropShadow {
            id: windowShadow
            z: pay_screen_wrapper.z-1
            anchors.fill: pay_screen_wrapper
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 16
            samples: 32
            color: "#aa000000"
            transparentBorder: true
            fast: true
            source: pay_screen_wrapper
    }
}
