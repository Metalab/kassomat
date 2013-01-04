import QtQuick 2.0

Rectangle{
    //property var ps
    property variant money_back: { 'fifty': 0, 'twenty': 0, 'ten': 0, 'five': 0, 'two': 0, 'one': 0, 'cfifty': 0, 'ctwenty': 0, 'cten': 0 };

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
        spacing: 10

        Component.onCompleted: {
            var component;
            var sprite;
            for (var prop in change_screen.money_back){
                component = Qt.createComponent("BubbleButton.qml");
                var bla = component.createObject(money_row,{
                                                "action": [function(){ parent.parent.state = "STANDARD_SCREEN" }],
                                                "width": 64,
                                                "height": 64,
                                                //"ps": ps,
                                                "img_id.source": "mate.png"
                                            });
            }
        }

    }
}
