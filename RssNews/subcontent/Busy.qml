import QtQuick 2.0

Image{
    id: busyContainer

    source: "./icons/busy.png"

    NumberAnimation on rotation{
        running: busyContainer.visible
        from: 0; to: 360
        loops: Animation.Infinite
        duration: 1200
    }
}
