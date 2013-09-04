import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    property var money_values: ['5000','2000','1000','500',
        '200','100','50','20','10'];
    property var money_selected: 0
    //property var money_selected: {'5000': 0,'2000': 0,'1000': 0,'500':0, '200':0,'100':0,'50':0,'20':0,'10':0};

    id: change_screen_wrapper
    color: "#eeeeee"

    MouseArea{
        anchors.fill: parent;
    }    

    signal refresh()
    onRefresh: {
        var x=0;

        for(var i=0; i < buttons.count; i++){
            x += buttons.itemAt(i).selected * buttons.itemAt(i).value;
        }

        console.log('selected value: '+ x);
        money_selected=x;
    }

    Component.onCompleted: {
        for(var i=0; i < buttons.count; i++){
            console.log(buttons.itemAt(i) + " connected") //.countchanged.connect(refresh);
            buttons.itemAt(i).countChanged.connect(refresh)
        }
    }

    Rectangle{
        id: change_screen
        anchors.margins: 15
        anchors.fill: parent
        color: "transparent"

        Row{
            id:money_row
            spacing: 5

            Text{
                text: controller.credit
                font.pixelSize: 72
            }

            Repeater {
                id: buttons
                model: money_values

                delegate: ChangeButton {
                    nomore: {
                        //console.log(modelData+' '+value+' <= '+controller.credit+' - '+money_selected)
                        //console.log((value <= (controller.credit-money_selected))? true : false)
                        return ((value <= (controller.credit-money_selected)))? true : false
                    }

                    visible:{
                        return ((value <= (controller.credit-money_selected)) || selected > 0)? true : false

                    }

                    color: "transparent"
                    value: modelData
                    onCountChanged: {
                        console.log('Die Anzahl von ' + value + ' ist jetzt ' + selected)
                    }
                }
            }
        }

        Button{
            id: fill_up_button
            height: 40
            width: 200

            label: 'fill up'

            color: "#444422";
            anchors.top: money_row.bottom

            onButtonClick: {

            }
        }

        Button{
            id: accept_button
            height: 40
            width: 200

            label: 'accept'

            color: "#444422";

            anchors.top: money_row.bottom
            anchors.left: fill_up_button.right

            onButtonClick: {

                for(var i=0; i <= buttons.count; i++){

                }

                console.log('hallo: '+ money_selected[5000] );

            }
        }
    }

    DropShadow {
            id: windowShadow
            z: change_screen_wrapper.z-1
            anchors.fill: change_screen_wrapper
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 16
            samples: 32
            color: "#aa000000"
            transparentBorder: true
            fast: true
            source: change_screen_wrapper
    }
}


