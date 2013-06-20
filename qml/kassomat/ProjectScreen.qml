import QtQuick 2.0
import QtGraphicalEffects 1.0
import db.product 1.0

Rectangle{
    id: project_screen_wrapper

    MouseArea{
                anchors.fill: parent;
    }

    ListView {
        id: list_view

        anchors.fill: parent
        model: productlist
        delegate:
            Rectangle{
                property variant myData: model
                height: 20
                width: 200
                color: "#CCCCCC"
                Text {
                   text: {
                       console.log("qml listview:")
                       for(var bla in myData)
                            console.log(bla);
                       return myData.display_name
                       //return "stub" //list_view.currentItem.data.display_name
                   }
                }
            }

        Component.onCompleted: {
            console.log("qml:");
            console.log(productlist);

        }
    }


}
