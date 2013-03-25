import QtQuick 2.0

Rectangle {
    property var money_back: ['5000','2000','1000','500',
        '200','100','50','20','10','5','2','1'];

    id: change_screen
    visible: (opacity == 0.0)? false : true;

    Behavior on opacity{
        NumberAnimation {
            easing.type: Easing.InOutExpo
            duration:1000
        }
    }
    anchors.fill: parent

    color: "#eeeeee"
    border.width: 1
    border.color: "black";

    anchors.margins: 10

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

//        action: [function(){
//            //fill up until no credits are left
//            //from biggest to smallest money

//        }]
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

//        action: [function(){

//            //go back
//            parent.parent.state = "STANDARD_SCREEN"
//        }]
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

//        action: {
//            //spit out money

//            //go back
//            parent.parent.state = "STANDARD_SCREEN"
//        }
    }
}
