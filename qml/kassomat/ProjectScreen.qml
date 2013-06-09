import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle{
    id: project_screen_wrapper

    MouseArea{
                anchors.fill: parent;
    }

    ListView {
        id: list_view

        anchors.fill: parent
        //model: qmlModel
        model: projectlist
        delegate: Rectangle {
           height: 20
           width: 200
           color: colour
           Text { text: name }
        }
    }


}
