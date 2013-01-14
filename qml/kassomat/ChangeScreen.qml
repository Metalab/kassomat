import QtQuick 2.0

Rectangle{
    //property var ps
    property variant money_back: { '50e': 0, '20e': 0, '10e': 0, '5e': 0, '2e': 0, '1e': 0, '50c': 0, '20c': 0, '10c': 0, '5c': 0, '2c': 0, '1c': 0 };

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
                                                //"ps": ps,
                                                "img_id.source": prop+".png",
                                                     "img_id.align": "center"
                                            });
            }
        }

    }
}
