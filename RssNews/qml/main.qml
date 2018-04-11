import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.XmlListModel 2.0
import "./subcontent"

Window {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Rss News")

    property string currentFeed: rssModel.get(0).feed
    property bool loading: feedModel.status === XmlListModel.Loading
    property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation

    onLoadingChanged: {
        if(feedModel.statis === XmlListModel.Ready)
            feeds.positionViewAtBeginning()
    }

    Rss{
        id: rssModel
    }

    XmlListModel{
        id: feedModel
        source: "http://" + window.currentFeed
        query: "/rss/channel/item"

        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "description"; query: "fn:replace(description/string(),'\&lt;a href=.*\/a\&gt;', '')" }
        XmlRole { name: "link"; query: "link/string()" }
        XmlRole { name: "pubDate"; query: "pubDate/string()" }
    }

    ListView{
        id: categorize
        property int itemWidth: 100

        width: isPortrait ? parent.width : itemWidth
        height: isPortrait ? itemWidth : parent.height
        orientation: isPortrait ? ListView.Horizontal : ListView.Vertical
        anchors.top: parent.top
        model: rssModel
        spacing: 3
        delegate: CategoryDelegate { itemSize: categorize.itemWidth }
    }

    ListView{
        id: feeds

        anchors.leftMargin: 160
        anchors.rightMargin: 4
        anchors.left: categorize.left
        anchors.top: window.top
        anchors.bottom: window.bottom
        clip: isPortrait
        model: feedModel
        delegate: NewsDelegate { }
    }

    Image{
        id: closeButton
        source: "./subcontent/icons/close.png"
        scale: 0.8
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 4
        width: 50
        height: 50

        MouseArea{
            anchors.fill: parent
            onClicked: {
                Qt.quit()
            }
        }
    }
}
