import QtQuick 2.0

Column{

    Title{
        label: "PROJECTS"

        Row{
            anchors.rightMargin: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Button{
                id:list
                borderColor: "#ffffff"
                border.width: 3
                buttonHeight:50
                buttonWidth: height
                buttonColor: "transparent"
                onHoverColor: "#ffffff"
                labelSize: 24
                label: "\u2263"

                onButtonClick: function(){
                    controller.state = "STANDARD_SCREEN";
            }
            }

            Button{
                id:backward
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
                id:forward
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
