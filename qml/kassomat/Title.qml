import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle{
    property var label

    width: parent.width
    height:70

    color: "#F15A24"

    Text{
        id: txt
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        smooth: true
        text: label
        font.bold: true
        font.pointSize: 28
        color: "white"
    }

    DropShadow{
            id: txtShadow;
            anchors.fill: txt
            cached: true;
            horizontalOffset: 3;
            verticalOffset: 3;
            radius: 8.0;
            samples: 16;
            color: "#80000000";
            smooth: true;
            source: txt;
    }
}
