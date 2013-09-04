import QtQuick 2.1

Column{
    spacing:5

    Title{
        label: "PAY ALL THE JOBS"
    }

    Row{
        width:parent.width
        height: 130
        spacing:5

        Button{
            buttonHeight:parent.height
            buttonWidth:(parent.width/parent.children.length)-2.5
            buttonColor: "#eeeeee"

            label:'Laser'
            labelSize:32

            onButtonClick: {

            }
        }
        Button{
            buttonHeight:parent.height
            buttonWidth:(parent.width/parent.children.length)-2.5
            buttonColor: "#eeeeee"

            label:'Vinyl'
            labelSize:32


            onButtonClick: {

            }
        }
        Button{
            buttonHeight:parent.height
            buttonWidth:(parent.width/parent.children.length)-2.5
            buttonColor: "#eeeeee"

            label:'Drucker'
            labelSize:32


            onButtonClick:{

            }
        }
    }
}
