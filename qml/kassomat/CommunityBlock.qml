import QtQuick 2.0

Column{

    Rectangle{
        width: parent.width
        height:70
        color: "#F15A24"

        Text{
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            font.pointSize: 28

            smooth:true
            text: "COMMUNITY"
            color: "#ffffff"
        }

        Row{
            anchors.rightMargin: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            Rectangle{
                id:list_c
                border.color: "#ffffff"
                border.width: 3
                height:50
                width: height
                color: "transparent"

                Text{
                    anchors.centerIn: parent
                    font.pointSize: 24
                    //text: "\ud834\udf6b"
                    text: "\u2263"
                    font.bold: true
                    color:"#ffffff"
                }
            }
            Rectangle{
                id:backward_c
                border.color: "#ffffff"
                border.width: 3
                height:50
                width: height
                color: "transparent"

                Text{
                    anchors.centerIn: parent
                    font.pointSize: 24
                    text: "\u25c0"
                    color:"#ffffff"
                }
            }
            Rectangle{
                id:forward_c
                border.color: "#ffffff"
                border.width: 3
                height:50
                width: height
                color: "transparent"

                Text{
                    anchors.centerIn: parent
                    font.pointSize: 24
                    text: "\u25b6"
                    color:"#ffffff"
                }
            }

        }
    }

    Rectangle{
        color:"#eeeeee"
        border.color: "#000000"
        border.width: 1

        width: parent.width
        height: 270
    }

}
