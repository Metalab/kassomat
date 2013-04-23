import QtQuick 2.0

Column{
    spacing:10

    Text{
        id: username_status
        anchors.horizontalCenter: parent.horizontalCenter

        font.bold: true
        font.pointSize: 16

        smooth:true
        text: "Anon User"
        color: "#ffffff"
    }

    Text{
        id: credits_status
        anchors.horizontalCenter: parent.horizontalCenter

        font.bold: true
        font.pointSize: 24

        smooth:true
        text: "Credits\n€ "+((controller.credit/100).toFixed(2))
        color: "#ffffff"
    }

    Button{
        id: change_button
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#00cccc"
        label: "change"

        onButtonClick: {
            controller.state = "CHANGE_SCREEN"
        }
    }

    Button{
        id: donate_button
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#00cccc"
        label: "donate"

        onButtonClick: {

        }
    }

    Button{
        id: redeem_button

        anchors.horizontalCenter: parent.horizontalCenter

        color: "#00cccc"
        label: "redeem"

        onButtonClick: {

        }
    }

    Rectangle{
        id:space
        anchors.left: parent.left
        enabled: false;
        height:220
        width:1
        color: "transparent"
    }

    Button{
        id: return_button
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#00cccc"
        label: "return"

        onButtonClick: {
            controller.state = "STANDARD_SCREEN";
        }
    }
}

