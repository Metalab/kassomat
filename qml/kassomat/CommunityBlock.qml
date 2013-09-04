import QtQuick 2.1

Column{

    Title{
        label: "COMMUNITY"

        Row{
            anchors.rightMargin: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Button{
                id:list_c
                borderColor: "#ffffff"
                border.width: 3
                buttonHeight:50
                buttonWidth: height
                buttonColor: "transparent"
                onHoverColor: "#ffffff"
                labelSize: 24
                label: "\u2263"
            }

            Button{
                id:backward_c
                borderColor: "#ffffff"
                border.width: 3
                buttonHeight:50
                buttonWidth: height
                buttonColor: "transparent"
                onHoverColor: "#ffffff"
                labelSize: 24
                label: "\u25c0"

            }

            Button{
                id:forward_c
                borderColor: "#ffffff"
                border.width: 3
                buttonHeight:50
                buttonWidth: height
                buttonColor: "transparent"
                onHoverColor: "#ffffff"
                labelSize: 24
                label: "\u25b6"
            }

        }
    }

    Rectangle{
        id: content
        color:"#eeeeee"

        width: parent.width
        height: 270
    }

}
