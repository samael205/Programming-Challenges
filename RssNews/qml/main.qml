import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.XmlListModel 2.0
import Qt.labs.settings 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import "./subcontent"

Window {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Rss News")

    property string currentFeed: rssModel.get(0).feed
    property bool loading: feedModel.status === XmlListModel.Loading
    property string datastore: ""

    ToolButton {
        action: deleteItem
    }

    Dialog{
        id: acceptRemoveAction
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        visible: false

        onAccepted: {
            rssModel.remove(categorize.currentIndex)
            visible =  false
        }
    }

    Action{
        id: deleteItem
        shortcut: "Del"
        onTriggered: {
            acceptRemoveAction.title = "Remove?"
            acceptRemoveAction.visible = true
        }
    }

    onLoadingChanged: {
        if(feedModel.statis === XmlListModel.Ready)
            feeds.positionViewAtBeginning()
    }

    Rss{
        id: rssModel
    }

    Component.onDestruction: {
        var datamodel = []
        for(var i=0; i<rssModel.count; ++i)
            datamodel.push(rssModel.get(i))
        datastore = JSON.stringify(datamodel)
    }

    NewRssDialog{
        id: newRss
        onAdded: {
            rssModel.append({ "name": newRss.category, "feed": newRss.feedUrl, "image" : newRss.iconUrl})
        }

        onEdited: {
            var currentIndex = categorize.currentIndex

            var name = newRss.category
            var feed = newRss.feedUrl
            var icon = newRss.iconUrl

            rssModel.set(currentIndex, {"name" : name, "feed" : feed, "image" : icon })
            newRss.editMode = false
        }

        onRejected: {
            newRss.editMode = false
        }
    }

    Component.onCompleted: {
        if(datastore){
            rssModel.clear()
            var datamodel = JSON.parse(datastore)
            for(var i = 0; i<datamodel.length; ++i)
                rssModel.append(datamodel[i])
        }
    }

    XmlListModel{
        id: feedModel
        source: window.currentFeed
        query: "/rss/channel/item"


        XmlRole { name: "title"; query: "title/string()" }
        XmlRole { name: "description"; query: "fn:replace(description/string(),'\&lt;a href=.*\/a\&gt;', '')" }
        XmlRole { name: "link"; query: "link/string()" }
        XmlRole { name: "pubDate"; query: "pubDate/string()" }
    }


    ListView{
        id: categorize
        property int itemWidth: 100

        width: parent.width
        height: parent.height
        orientation:  ListView.Vertical
        anchors.top: parent.top
        model: rssModel
        spacing: 3
        delegate: CategoryDelegate { itemSize: categorize.itemWidth }
    }

    Settings{
        property alias datastore: window.datastore
    }

    ListView{
        id: feeds
        anchors.leftMargin: 160
        anchors.rightMargin: 4
        anchors.left: categorize.left
        anchors.top: window.top
        anchors.bottom: window.bottom
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

    Image{
        id: addRss
        source: "./subcontent/icons/add.png"
        scale: 0.9
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 45
        height: 45

        MouseArea{
            anchors.fill: parent
            onClicked:{
                newRss.iconUrl = "./icons/none.png"
                newRss.setText("", "")
                newRss.visible = true
            }
        }
    }

    function editCurrentRSS(){
        var currentIndex = categorize.currentIndex
        var currentRss = rssModel.get(currentIndex)
        newRss.setText(currentRss.feed, currentRss.name)
        newRss.iconUrl = currentRss.image
        newRss.editMode = true
        newRss.visible = true
    }

    Image{
        id: editRss
        source: "./subcontent/icons/edit.png"
        scale: 0.9
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.bottom: parent.bottom
        width: 45
        height: 45

        MouseArea{
            anchors.fill: parent
            onClicked: editCurrentRSS()
        }
    }
}
