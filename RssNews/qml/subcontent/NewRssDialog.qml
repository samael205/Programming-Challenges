import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

Dialog{
    id: mainDialog
    visible: false
    title:  editMode ? "Edit RSS" : "New RSS"
    standardButtons: Dialog.Ok | Dialog.Cancel

    property bool editMode: false

    property string iconUrl: ""
    property string category: categoryField.text
    property string feedUrl: feedField.text

    signal added
    signal edited

    function setText(url, category){
        categoryField.text = category
        feedField.text = url
    }

    contentItem: Rectangle{
        id: content
        color: "lightblue"
        implicitWidth: 350
        implicitHeight: 150

        TextField{
            id: categoryField
            placeholderText: qsTr("Enter Category")
            anchors.top: parent.top
            width: parent.width
        }

        TextField{
            id: feedField
            placeholderText: qsTr("Enter Feed URL")
            anchors.top: categoryField.bottom
            anchors.topMargin: 10
            width: parent.width
        }

        Image{
            id: addIcon
            source: "./icons/addIcon.png"
            width: 70
            height: 70
            scale: 0.7
            anchors.bottom: parent.bottom

            MouseArea{
                anchors.fill: parent
                onClicked: addFile.visible = true
            }

            FileDialog{
                id: addFile
                visible: false
                folder: shortcuts.pictures
                nameFilters: ["*.png", "*.jpg", "*.jpeg", "*.svg"]
                onAccepted: iconUrl = addFile.fileUrl
            }
        }

       Image{
           id: iconShow
           source: iconUrl
           width: 50
           height: 50
           scale: 0.7
           anchors.bottom: parent.bottom
           anchors.left: parent.left
           anchors.leftMargin: 80
       }

       Image{
            source: "./icons/cancel.png"
            width: 70
            height: 70
            scale: 0.7
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 60

            MouseArea{
                anchors.fill: parent
                onClicked:{
                    mainDialog.visible = false
                    if(mainDialog.editMode)
                            mainDialog.editMode = false
                }
            }
       }

        Image{
            source: "./icons/ok.png"
            width: 70
            height: 70
            scale: 0.7
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            MouseArea{
                anchors.fill: parent
                onClicked:{
                    if(category.length != 0 && feedUrl.length != 0 && editMode == false)
                        added()
                    if(editMode && feedUrl.length != 0)
                        edited()
                    mainDialog.visible = false
                }
            }
        }
    }
}
