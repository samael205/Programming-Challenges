import QtQuick 2.0

Rectangle{
    id: delegate

    property url currentSongSource

    signal clicked(url currentSource)

    height: 12
    width: 200
    anchors.right: parent.right
    anchors.rightMargin: 30
    color: "transparent"

    function getSongName(source){
        var lastElement = source.split("/")
        lastElement = lastElement[lastElement.length-1]
        var splitted = lastElement.split("-")
        return splitted[1]
    }

    Text{
        anchors.left: parent.left
        font.pointSize: 7
        font.bold: true
        color: (currentSongSource == source) ? "red" : "#7a0099"
        text: getSongName(source.toString())

        MouseArea{
            anchors.fill: parent
            onClicked: {
                delegate.clicked(source)
            }
        }
    }
}
