import QtQuick 2.1


Rectangle {
    id: changeButton
    property int selected: 0; // anzahl der scheine/muenzen
    property int value: 0; // geldwert
    property bool nomore;
    property bool notless: selected;
    signal countChanged(int value, int selected)

    width: button_pic.sourceSize.width
    height: button_pic.sourceSize.height

    Image {
        id: button_pic
        source: changeButton.value + '.png'
    }

    Column{
        spacing:5

        Rectangle{
            id:button_increase
            width: plus.sourceSize.width
            height: plus.sourceSize.height
            color: "transparent"

            visible: nomore

            MouseArea {
                id: plus_area

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button == Qt.LeftButton) {
                        changeButton.selected += 1;
                        changeButton.countChanged(changeButton.value, changeButton.selected);
                    }
                }
            }

            Image {
                id: plus
                source: 'changebutton_plus.png'
            }

        }

        Rectangle{
            id:button_decrease
            width: minus.sourceSize.width
            height: minus.sourceSize.height
            color: "transparent"

            visible: notless

            MouseArea {
                id: minus_area

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button == Qt.LeftButton) {
                        changeButton.selected -= 1;
                        changeButton.countChanged(changeButton.value, changeButton.selected);
                    }
                }
            }

            Image {
                id: minus
                source: 'changebutton_minus.png'
            }

        }
    }

    Text {
        text: selected
        color: "red"
    }
}
