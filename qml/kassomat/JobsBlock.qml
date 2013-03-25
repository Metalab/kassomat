import QtQuick 2.0

Column{
    spacing:5

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
            text: "PAY ALL THE JOBS"
            font.bold: true
            color: "white"
        }
    }

    Row{
        width:parent.width
        height: 130
        spacing:5

        Button{
            height:parent.height
            width:(parent.width/3)-2.5
            border.color: "#000000"
            border.width: 1

            onButtonClick: {

            }
        }
        Button{
            height:parent.height
            width:(parent.width/3)-2.5
            border.color: "#000000"
            border.width: 1

            onButtonClick: {

            }
        }
        Button{
            height:parent.height
            width:(parent.width/3)-2.5
            border.color: "#000000"
            border.width: 1

            onButtonClick: {

            }
        }
    }
}
