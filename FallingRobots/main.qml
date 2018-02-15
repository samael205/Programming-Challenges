import QtQuick 2.5
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 450
    title: qsTr("Robots!")

    Image{
        anchors.centerIn: parent
        id: background
        source: "/icons/space.png"
      }
        Text {
            id: backgroundText
            anchors.centerIn: parent
            text: qsTr("Hello, Robots!")
            font{
                family: "Verdana"
                bold: true
                pixelSize: 30
            }
            color: "#CC8B00"
        }

    DropShadow{
        anchors.fill: source
        cached: true
        horizontalOffset: 1
        verticalOffset: 1
        radius: 1
        samples: 14
        smooth: true
        source: backgroundText
    }

    Item{
        id: robot1

        SequentialAnimation on y{
            loops: Animation.Infinite
            PropertyAnimation { to: mainWindow.height-100; duration: 4000}
            PropertyAnimation { to: 0; duration: 6000 }
        }

        BorderImage {
            source: "/icons/robot.png"
            width: 100; height: 100
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
        }
    }

    Item{
        id: robot2
        x: 350

        SequentialAnimation on y{
            loops: Animation.Infinite
            PropertyAnimation { to: mainWindow.height-100; duration: 6000 }
            PropertyAnimation { to: 0; duration: 8000 }
        }

        BorderImage {
            source: "/icons/robot.png"
            width: 100; height: 100
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
        }

    }

    Item{
        id: robot3
        x: 550

        SequentialAnimation on y{
            loops: Animation.Infinite
            PropertyAnimation { to: mainWindow.height-100; duration: 4000 }
            PropertyAnimation { to: 0; duration: 2000 }
        }

        BorderImage {
            source: "/icons/robot.png"
            width: 100; height: 100
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
        }
    }
}
