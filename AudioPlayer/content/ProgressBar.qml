import QtQml 2.2
import QtQuick 2.0
import QtGraphicalEffects 1.0

Canvas{
    id: progressBar
    width: 240
    height: 240
    antialiasing: true

    property real centerWidth: width/2
    property real centerHeight: height/2
    property real radius: Math.min(progressBar.width, progressBar.height) / 2

    property real minimumValue: 0
    property real maximumValue: 100
    property real currentProgress: 0

    property bool showCurrentAudioTimeOrDuration: true

    property real angle: (currentProgress - minimumValue) / (maximumValue - minimumValue) * 2 * Math.PI

    property real angleOffset: -Math.PI / 2

    property string text: "\0"

    signal clicked()

    onMaximumValueChanged: requestPaint()
    onMinimumValueChanged: requestPaint()
    onCurrentProgressChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.save()

        ctx.clearRect(0, 0, progressBar.width, progressBar.height)

        ctx.beginPath()
        ctx.lineWidth = 3
        ctx.strokeStyle = "red"
        ctx.arc(progressBar.centerWidth,
                progressBar.centerHeight,
                progressBar.radius,
                progressBar.angleOffset,
                progressBar.angleOffset + progressBar.angle)
        ctx.stroke()
        ctx.restore();
       }

        Text{
            id: songStatus
            anchors.centerIn: parent
            text: timeFormatDuration((showCurrentAudioTimeOrDuration) ? mediaPlayer.position : mediaPlayer.duration)
            color: "#3DF736"
            font.pointSize: 18

            NumberAnimation on font.pointSize{
                id: textToUpper
                from: 1
                to: 18
                duration: 100
            }

            NumberAnimation on font.pointSize{
                id: textToLower
                from: 18
                to: 1
                duration: 100
            }

        }

        DropShadow{
            anchors.fill: songStatus
            source: songStatus
            color: "#000000"
            radius: 4
            samples: 9
            verticalOffset: 0
            horizontalOffset: 2
            spread: 0
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                textToLower.start()
                showCurrentAudioTimeOrDuration = !showCurrentAudioTimeOrDuration
                textToUpper.start()
            }
        }
}

