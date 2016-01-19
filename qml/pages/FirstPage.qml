/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }
        }

        contentHeight: mainarea.height

        Rectangle {
            id: mainarea
            width: page.width; height: page.height
            color: "#150A00"

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/3.84
                font.family: "Sans serif";
                font.pixelSize: page.height/12;
                font.bold: true
                text: "SailWORM"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/2.286
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "A simple worm game"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y: page.height/2.043
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "Three different game modes"
                color: "#ffff80"
            }


            Rectangle {
                id: rowrec
                x: page.width/27
                y: page.height/1.477
                color: "#150A00"
                Row {
                    spacing: page.width/21.6
                    anchors.fill: rowrec

                    Rectangle {
                        height: page.height/6.62
                        width: page.width/3.6
                        color: "#150A00"
                        Rectangle {
                            height: page.height/7.68
                            width: page.width/3.6
                            color: "#150A00"
                            Image {
                                id: origimg
                                source: "original.jpg"
                                anchors.fill: parent
                            }
                        }

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            y: page.height/8.205
                            color: "#ffff80"
                            text: "Original"
                            font.pixelSize: page.height/38.4
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                pageStack.replace(Qt.resolvedUrl("SecondPage.qml"))
                            }
                        }
                    }

                    Rectangle {
                        height: page.height/6.62
                        width: page.width/3.6
                        color: "#150A00"
                        Rectangle {
                            height: page.height/7.68
                            width: page.width/3.6
                            color: "#150A00"
                            Image {
                                id: chaseimg
                                source: "chase.jpg"
                                anchors.fill: parent
                            }
                        }

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            y: page.height/8.205
                            color: "#ffff80"
                            text: "Chase mode"
                            font.pixelSize: page.height/38.4
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                pageStack.replace(Qt.resolvedUrl("FourthPage.qml"))
                            }
                        }
                    }

                    Rectangle {
                        height: page.height/6.62
                        width: page.width/3.6
                        color: "#150A00"
                        Rectangle {
                            height: page.height/7.68
                            width: page.width/3.6
                            color: "#150A00"
                            Image {
                                id: duelimg
                                source: "duel.jpg"
                                anchors.fill: parent
                            }
                        }

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            y: page.height/8.205
                            color: "#ffff80"
                            text: "Duel mode"
                            font.pixelSize: page.height/38.4
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                pageStack.replace(Qt.resolvedUrl("ThirdPage.qml"))
                            }
                        }
                    }
                }
            }
        }
    }
}


