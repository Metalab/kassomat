import QtQuick 2.0

Rectangle{
    property var ps

    id: pay_screen
    visible: (opacity == 0.0)? false : true;
    Behavior on opacity{ NumberAnimation { easing.type: Easing.InOutExpo; duration: 1000 } }
    anchors.fill: parent

    color: "#eeeeee"
    border.width: 1
    border.color: "black";
    radius: 10

    anchors.margins: 10

    BubbleButton{
        id: accept_button
        height: 40
        width: 200

        color: "#444422";

//        ps:ps

        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        action: [function(){
            //pay with your current, thrown in, credit

            //change the text

            //delay

            //go back
            parent.parent.state = "STANDARD_SCREEN"
        }]
    }

    BubbleButton{
        id: cancel_button
        height: 40
        width: 200

        color: "#444422"

//        ps:ps

        anchors.left: accept_button.right
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        action: [function(){
            // spit out the money

            // and go back
            parent.parent.state = "STANDARD_SCREEN"
        }]
    }
}
