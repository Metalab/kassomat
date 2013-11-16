import QtQuick 2.0
import QtGraphicalEffects 1.0
import db.product 1.0
import QtQuick.Layouts 1.0

Rectangle{
    id: project_screen_wrapper

    MouseArea{
                anchors.fill: parent;
    }

    ListView {
        id: list_view

        anchors.fill: parent
        model: projectlist
        delegate:
            Rectangle{
                anchors.fill: parent
                color: "#CCCCCC"

                RowLayout{
                    anchors.fill: parent
                    spacing: 10

                    Image{
                        source: image_url
                    }

                    ColumnLayout{
                        spacing: 5
                        Layout.fillWidth: true

                        Text {
                            font.pixelSize: 32
                            font.bold: true
                            text: name
                        }

                        Text{
                            text: description
                        }
                    }

                    Button{


                        buttonWidth:80
                        buttonHeight:80
                        buttonColor:"#eeeeee"

                        label: "@"
                        labelSize: 40

                        onButtonClick: {
                            controller.state = "STANDARD_SCREEN";
                        }
                    }
                }
            }
    }


}
