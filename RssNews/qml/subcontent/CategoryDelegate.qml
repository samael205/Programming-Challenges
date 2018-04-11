import QtQuick 2.0

Rectangle{
    id: delegate

    property real itemSize
    property bool selected: ListView.isCurrentItem

    width: itemSize
    height: itemSize

    Image{
        anchors.centerIn: parent
        source: image
        width: itemSize
        height: itemSize
    }

    Text{
        id: titleText

        anchors.centerIn: parent
        style: Text.StyledText
        styleColor: "black"

        font{
            pixelSize: 13
            bold: true
        }
        text: name
        verticalAlignment: Qt.AlignCenter
        color: selected ? "red" : "orange"
        scale: selected ? 1.15 : 1.0
        Behavior on color { ColorAnimation {   duration: 200  } }
        Behavior on scale { PropertyAnimation { duration: 350 } }
    }

    Busy{
        scale: 0.8
        visible: delegate.ListView.isCurrentItem && window.loading
        anchors.centerIn: parent
        width: 70
        height: 70
    }

    MouseArea{
        anchors.fill: delegate
        onClicked: {
            delegate.ListView.view.currentIndex = index
                if(window.currentFeed == feed)
                    feedModel.reload()
                else
                    window.currentFeed = feed
        }
    }
}
