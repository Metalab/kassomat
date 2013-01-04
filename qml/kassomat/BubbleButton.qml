import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0

Rectangle {
    property alias img_id: button_pic
    property var action
    //property var ps

    border.width: 1
    border.color: "black";
    radius: 10
    clip: true

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button == Qt.LeftButton){
                emit.pulse(500);
                action[0]();
            }
        }
    }

    Image {
        id: button_pic

        ParticleSystem{
            id:ps
        }

        ImageParticle {
            anchors.fill: parent
            system: ps
            source: "bubble2.png"
            sizeTable: "size.png"
            alpha: 10
            colorVariation: 0.2
        }
        Emitter {
            id: emit
            anchors.fill: parent
            system: ps
            emitRate: 50
            lifeSpan: 1500
            lifeSpanVariation: 1000
            size: 32
            sizeVariation: 10
            velocity: AngleDirection {angle:-90; angleVariation:20; magnitude: 180}
            enabled: false
        }
    }



}
