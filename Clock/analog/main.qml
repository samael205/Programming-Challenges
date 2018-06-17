import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    id: clock
    visible: true
    minimumWidth: 425
    maximumWidth: 425
    minimumHeight: 460
    maximumHeight: 460
    title: qsTr("Analog Clock")

    property int hours
    property int minutes
    property int seconds

    function timeChanged(){
        var date = new Date
        hours = date.getHours()
        minutes = date.getMinutes()
        seconds = date.getSeconds()
    }
    function isDay(){
        return hours > 7 && hours < 21
    }
    Timer{
        interval: 100; running: true; repeat: true;
        onTriggered: clock.timeChanged()
    }
    Item{
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        Image{
            id: background;
            source: isDay() ? "./content/day.jpg" : "./content/night.jpg"
            anchors.fill: parent
        }
        Image{
            x: 208
            y: 73
            source: "./content/second.png"
            transform: Rotation{
                id: secondsRotation
                origin.x: 2.5; origin.y: 155
                angle: clock.seconds * 6
                Behavior on angle{
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                }
            }
        }
        Image{
            x: 210
            y: 83
            source: "./content/minutes.png"
            transform: Rotation{
                id: minuteRotation
                origin.x: 6.5; origin.y: 154;
                angle: clock.minutes * 6
                Behavior on angle{
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                }
            }
        }
        Image{
            x: 210
            y: 100
            source: "./content/hours.png"
            transform: Rotation{
                id: hourRotation
                origin.x: 7.5; origin.y: 131
                angle: (clock.hours * 30) + (clock.minutes * 0.5)
                Behavior on angle{
                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360}
                }
            }
        }
    }
}
