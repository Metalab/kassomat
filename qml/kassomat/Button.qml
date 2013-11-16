/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    //identifier of the item
    id: button

    //these properties act as constants, useable outside this QML file
    property int buttonHeight: 75
    property int buttonWidth: 150

    property int position: 0

    property string label
    property color textColor: buttonLabel.color

    property color onHoverColor: "#7888AF"
    property color borderColor: "transparent"

    property color buttonColor: "#4F5C7F"

    property real labelSize: 20

    Component.onCompleted: {
        buttonLabel.anchors.centerIn = undefined;

        switch(position){
        case 0: buttonLabel.anchors.centerIn = button;
            break;
        case 100: buttonLabel.anchors.right = button.right
            buttonLabel.anchors.bottom = button.bottom
            buttonLabel.anchors.margins = 10;
            break;
        default: buttonLabel.anchors.centerIn = button;
            break;
        }
    }

    border { width: 2; color: borderColor }
    width: buttonWidth; height: buttonHeight

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        text: label

        color: "white"
        font.pointSize: labelSize
    }

    DropShadow{
            id: buttonLabelShadow;
            anchors.fill: buttonLabel
            cached: true;
            horizontalOffset: 3;
            verticalOffset: 3;
            radius: 8.0;
            samples: 16;
            color: "#80000000";
            smooth: true;
            source: buttonLabel;
    }

    //buttonClick() is callable and a signal handler, onButtonClick is automatically created
    signal buttonClick()

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        onClicked: buttonClick()

        hoverEnabled: true

        onPressed: SequentialAnimation {
            PropertyAction{ target: button; property: "z"; value: "100" }
            PauseAnimation{ duration: 55 }
            PropertyAction{ target: button; property: "z"; value: "100" }
        }
        onEntered: parent.border.color = onHoverColor
        onExited:  parent.border.color = borderColor
    }

    color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.1) : buttonColor
    Behavior on color { ColorAnimation{ duration: 55 } }

    scale: buttonMouseArea.pressed ? 1.1 : 1.00
    Behavior on scale { NumberAnimation{ duration: 55 } }

}
