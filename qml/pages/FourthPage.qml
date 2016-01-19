import QtQuick 2.0
import Sailfish.Silica 1.0
import wormengine 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    ChaseData {
        id: wormData
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: listView.height

        Rectangle {
            id: banner
            x: 0; y: 0
            width: page.width; height: page.height/5
            color: "#1a1300"
            opacity: 0.5

            Rectangle {
                x: (banner.width/2) - banner.width/13.5; y: banner.height/4.8;
                width: banner.width/18; height: banner.height/1.71;
                color: "white"
            }

            Rectangle {
                x: (banner.width/2) + banner.width/54; y: banner.height/4.8;
                width: banner.width/18; height: banner.height/1.71;
                color: "white"
            }

            Text {
                x: banner.width*(4/5); y: banner.height/3.5;
                font.family: "Sans serif";
                font.pixelSize: banner.height/2.75;
                text: wormData.apples - 30
                color: "white"
            }

            MouseArea {
                anchors.fill: banner
                onClicked: {
                    if (!wormData.isPaused) {
                        wormData.pauseMenu = true
                        wormData.isPaused = true
                    }
                }
            }
        }
        Item {
            id: main
            x: 0; y: page.height/5
            width: page.width; height: page.height * (8/10)
            enabled: !wormData.gameOver

            Rectangle {
                id: mainarea
                anchors.fill: main
                color: "#ffcc66"
                opacity: 0.7

                ListView {
                    id: listView
                    width: mainarea.width; height: mainarea.height
                    model: wormData.wormBody
                    delegate: component
                    Component {
                       id: component
                       Item {
                           id: item

                           Rectangle {
                               x: px; y: py
                               width: mainarea.height/9.6; height: mainarea.height/9.6
                               radius: (mainarea.height/9.6)/2
                               color: "#1a0000"
                               border.width: 0
                           }
                       }
                    }
                }

                Rectangle {
                    x: wormData.foodx; y: wormData.foody
                    width: (mainarea.height/9.6)/2; height: (mainarea.height/9.6)/2
                    radius: (mainarea.height/9.6)/4
                    color: "#990000"
                    border.width: 0
                }
                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        if (wormData.isPaused) {
                            wormData.isPaused = false
                        }
                        wormData.setTouch(mouseX-(mainarea.height/9.6)/2, mouseY-(mainarea.height/9.6)/2)
                    }

                    onPositionChanged: {
                        wormData.setTouch(mouseX-(mainarea.height/9.6)/2, mouseY-(mainarea.height/9.6)/2)
                    }
                }
            }
        }
    }
    Item {
        id: gameover
        visible: wormData.gameOver
        enabled: wormData.gameOver
        x: page.width/13.5; y: page.height/3.84
        width: page.width/1.174; height: page.height/2

        Rectangle {
            anchors.fill: gameover
            color: "red"
            opacity: 0.5

            Button {
                anchors.centerIn: parent
                text: "Game Over"
                color: "white"
                onClicked: {
                    wormData.newGame()
                }
            }
        }
    }

    Item {
        id: finish
        visible: wormData.finish
        enabled: wormData.finish
        x: (page.width/2) - 80; y: page.height/5
        width: page.width/3.375; height: page.height/32;

        Rectangle {
            anchors.fill: parent
            color: "white"

            Text {
                anchors.centerIn: parent
                text: "door"
                color: "black"
                font.pixelSize: page.height/42
            }
        }
    }

    Item {
        id: theend
        visible: wormData.theend
        enabled: wormData.theend
        x: page.width/13.5; y: page.height/3.84
        width: page.width/1.174; height: page.height/2

        Rectangle {
            anchors.fill: theend
            color: "green"
            opacity: 0.5

            Text {
                x: theend.width/7.67
                y: theend.height/16
                text: "Congratulation!\nYou finish the game!"
                color: "white"
            }

            Button {
                anchors.centerIn: parent
                text: "New game"
                color: "white"
                onClicked: {
                    wormData.newGame()
                }
            }
        }
    }

    Item {
        id: pausemenu
        visible: wormData.pauseMenu
        enabled: wormData.pauseMenu
        x: page.width/13.5; y: page.height/3.84
        width: page.width/1.174; height: page.height/2

        Rectangle {
            anchors.fill: pausemenu
            color: "#0f0f0a"
            opacity: 0.5

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                y: pausemenu.height/6
                text: "Reset game"
                color: "white"
                onClicked: {
                    wormData.newGame()
                    wormData.pauseMenu = false
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                y: pausemenu.height/2.18
                text: "Back to main"
                color: "white"
                onClicked: {
                    pageStack.replace(Qt.resolvedUrl("FirstPage.qml"))
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                y: pausemenu.height/1.33
                text: "Close menu"
                color: "white"
                onClicked: {
                    wormData.pauseMenu = false
                }
            }
        }
    }
}





