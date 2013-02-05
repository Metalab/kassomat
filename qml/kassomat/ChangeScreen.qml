import QtQuick 2.0

Rectangle{
    property var money_back: { '5000': 0,
                               '2000': 0,
                               '1000': 0,
                               '500': 0,
                               '200': 0,
                               '100': 0,
                               '50': 0,
                               '20': 0,
                               '10': 0,
                               '5': 0,
                               '2': 0,
                               '1': 0 };

    id: change_screen
    visible: (opacity == 0.0)? false : true;
    opacity: (parent.state == "CHANGE_SCREEN")
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

        Component.onCompleted: {
            var button = Qt.createComponent("ChangeButton.qml");
            var temp;
            for (var prop in change_screen.money_back){
                temp = button.createObject(money_row,{
                                                "action": [function(){ this.selected += 1; }],
                                                "img_id.source": prop+".png",
                                                "img_id.align": "center",
                                                "color": "transparent",
                                                "value": prop
                                            });

            }
        }

    }

    BubbleButton{
        id: fill_up_button
        height: 40
        width: 200

        color: "#444422";
        anchors.top: money_row.bottom

        action: [function(){
            //fill up until no credits are left
            //from biggest to smallest money

        }]
    }

    BubbleButton{
        id: accept_button
        height: 40
        width: 200

        color: "#444422";

        anchors.top: money_row.bottom
        anchors.left: fill_up_button.right

        action: [function(){

            //go back
            parent.parent.state = "STANDARD_SCREEN"
        }]
    }

    BubbleButton{
        id: cancel_button
        height: 40
        width: 200

        color: "#444422";
        anchors.top: money_row.bottom
        anchors.left: accept_button.right

        action: [function(){
            //spit out money

            //go back
            parent.parent.state = "STANDARD_SCREEN"
        }]
    }
}
