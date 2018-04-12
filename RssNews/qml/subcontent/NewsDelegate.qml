import QtQuick 2.0

Column{
    id: delegate
    width: 550
    spacing: 8

    Item{
        height: 8
        width: delegate.width
    }

    Row{
        spacing: 8
        width: parent.width

        Text{
            id: titleText
            width: delegate.width
            text: title
            wrapMode: Text.WordWrap
            font.pixelSize: 20
            font.bold: true
        }
    }

    Text{
        font.pixelSize: 12
        textFormat: Text.RichText
        font.italic: true
        anchors.right: titleText.left
        text:  pubDate + "  <a href=\"" + link + "\"> Show more!</a>"
        onLinkActivated: {
            Qt.openUrlExternally(link)
        }
    }

    Text{
        id: descriptionText
        text: description
        width: parent.width
        wrapMode: Text.WordWrap
        font.pixelSize: 14
        textFormat: Text.StyledText
        horizontalAlignment: Qt.AlignLeft
    }
}
