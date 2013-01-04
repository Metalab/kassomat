import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0

// bei bezahlen bildschirm, moeglichkeit geben zu spenden
// oder rest an ein kickstarter-projekt
// potentieller schotter auf ibutton raufladen wenn schotter gefahr besteht




Rectangle{
    id: app
    width: 800
    height: 600

    state: "STANDARD_SCREEN"

    states:[
            State {
                name: "STANDARD_SCREEN"
            },
            State {
                name: "PAY_SCREEN"
            },
            State {
                name: "CHANGE_SCREEN"
            }
        ]

//    ParticleSystem{
//        id: particles
//    }

    StandardScreen{
//        ps: particles
    }

    PayScreen{
//        ps: particles
    }

    ChangeScreen{
//        ps: particles
    }

}


