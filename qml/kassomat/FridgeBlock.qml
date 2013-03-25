import QtQuick 2.0

Column{
    spacing: 5

    Rectangle{
        width: parent.width
        height:70

        color: "#F15A24"

        Text{
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 28

            smooth: true
            text: "METAFRIDGE"
            font.bold: true
            color: "white"
        }
    }

    Button{
        width:parent.width
        height:140
        border.color: "#000000"
        border.width: 1

        onButtonClick: {

        }
    }

    Button{
        width:parent.width
        height:140
        border.color: "#000000"
        border.width: 1

        onButtonClick: {

        }
    }

    Row{
        width:parent.width
        height:140

        spacing: 5

        Button{
            height:parent.height
            width: (parent.width/2)-2.5

            border.color: "#000000"
            border.width: 1

            onButtonClick: {

            }
        }

        Button{
            height:parent.height
            width:(parent.width/2)-2.5
            border.color: "#000000"
            border.width: 1

            onButtonClick: {

            }
        }
    }
}
