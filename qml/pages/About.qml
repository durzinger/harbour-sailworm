import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    id: page

    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: aboutpage.height

        Rectangle {
            id: aboutpage
            width: page.width; height: page.height
            color: "#150A00"
            opacity: 0.9

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/4.8
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "Code and"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y: page.height/3.84
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "design by:"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/3.2
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "Mikko Manninen"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/2.742
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "email:"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/2.4
                font.family: "Sans serif";
                font.pixelSize: page.height/32
                text: "mikkomanninen79@gmail.com"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/1.92
                font.family: "Sans serif";
                font.pixelSize: page.height/38.4
                text: "Git:"
                color: "#ffff80"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:page.height/1.7454
                font.family: "Sans serif";
                font.pixelSize: page.height/38.4
                text: "github.com/mikkom79/sailfish"
                color: "#ffff80"
            }

        }
    }
}

