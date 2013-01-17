import QtQuick 2.0

Rectangle{
    //property var ps
    property var money_back: { '50e': 0,
                               '20e': 0,
                               '10e': 0,
                               '5e': 0,
                               '2e': 0,
                               '1e': 0,
                               '50c': 0,
                               '20c': 0,
                               '10c': 0,
                               '5c': 0,
                               '2c': 0,
                               '1c': 0 };

    id: change_screen
    visible: (opacity == 0.0)? false : true;
    opacity: (parent.state == "CHANGE_SCREEN")
    Behavior on opacity{ NumberAnimation { easing.type: Easing.InOutExpo; duration:1000; } }
    anchors.fill: parent

    color: "#eeeeee"
    border.width: 1
    border.color: "black";

    anchors.margins: 10



    Row{
        id:money_row
        spacing: 5
    BubbleButton{
        id: button_5e
        action: [function(){ selected_5e.selected += 1; }]
        img_id.source: "5e.png"
        color: transparent
    }
    ChangeText{
        id: selected_5e
    }
//        Component.onCompleted: {
//            var button = Qt.createComponent("BubbleButton.qml");
//            var selected = Qt.createComponent("ChangeText.qml");
//            var temp;
//            for (var prop in change_screen.money_back){
//                temp = selected.createObject(money_row,{
//                                          "id": "selected_"+prop,
//                                          "selected": "0"
//                                        });

//                temp = button.createObject(money_row,{
//                                                "id": "button_"+prop,
//                                                "action": [function(){ eval("parent.selected_"+prop).selected = 1; }],
//                                                //"ps": ps,
//                                                "img_id.source": prop+".png",
//                                                "img_id.align": "center",
//                                                "color": "transparent"
//                                            });
////                console.log("3rd "+JSON.stringify(temp, null, 4));
////                console.log(prop);

//            }
//        }
    }

    BubbleButton{
        id: fill_up_button
        height: 40
        width: 200

        color: "#444422";

//        ps:ps
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

//        ps:ps
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

//        ps:ps
        anchors.top: money_row.bottom
        anchors.left: accept_button.right


        action: [function(){
            //spit out money

            //go back
            parent.parent.state = "STANDARD_SCREEN"
        }]
    }
}
