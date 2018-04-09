import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtMultimedia 5.9
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "./content"

Item{
    id: mainview
    width: 1215
    height: 720
    visible: true
    property bool musicPlaying: false
    property var noImageRepeat
    property int currentMusicIndex: 0

    FolderListModel{
        id: folderModel
        nameFilters: ["*.jpg"]
        folder: "./content/images"
    }

    Image{
        id: wallpaper
        anchors.fill: parent
        source: "./content/images/anim0.jpg"
        NumberAnimation on opacity{
            id: fadeInAnimation
            from: 0
            to: 1
            duration: 1000
        }

    }

    Timer{
        interval: 10000
        running: musicPlaying
        repeat: true
        onTriggered: getNextImage()
    }

    function getNextImage(){
        var numberOfImages = folderModel.count
        var getIndexOfImages = Math.floor(Math.random() * numberOfImages);

        while(noImageRepeat === getIndexOfImages)
            getIndexOfImages = Math.floor(Math.random() * numberOfImages);

        noImageRepeat = getIndexOfImages;
        var imagePath = folderModel.get(getIndexOfImages, "fileURL")
        wallpaper.source = imagePath;
        fadeInAnimation.start();
    }

    Playlist{
        id: playlist
    }

    function setAudioIndex(index){
        mediaPlayer.source = playlist.itemSource(index)
    }

    function changeMusicWhenSongEnded(){
        currentMusicIndex++;
        if(currentMusicIndex >= playlist.itemCount)
            currentMusicIndex = 0;
        setAudioIndex(currentMusicIndex);
    }

    MediaPlayer{
        id: mediaPlayer
        autoLoad: true
        autoPlay: true
        volume: 0.8
        onError: console.error("Error with audio " + mediaPlayer.error)

        onStatusChanged: {
            if(status == MediaPlayer.EndOfMedia)
                changeMusicWhenSongEnded()
        }

        onPositionChanged: {
            progressChanged.start()
        }

        onPlaying: {
            (!musicPlaying)
                musicPlaying = true
        }
    }

    function songNameAndAuthor(){
        if(!mediaPlayer.hasAudio)
            return ""
        var getSong = mediaPlayer.source.toString()
        var getName = getSong.split("/")
        getName = getName[getName.length - 1]
        var songInfo = getName.split("-")
        var title = songInfo[1]
        var author = songInfo[0]
        return author + "\n" + title
    }

    function timeFormatDuration(milliseconds){
        var seconds = parseInt((milliseconds / 1000) % 60)
        var minutes = parseInt(((milliseconds / (1000*60)) % 60))
        if(seconds < 10)
            return minutes + ":0" + seconds
        else
            return minutes + ":" + seconds
    }

    ProgressBar{
        id: songProgress
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        property int previousPositionStatus

        NumberAnimation on currentProgress {
            id: progressChanged
            to: (mediaPlayer.position * 100) / mediaPlayer.duration
            duration: 500
        }
    }

    FileDialog{
        id: fileDialog
        title: "Set your audio"
        folder: shortcuts.music
        selectMultiple: true
        onAccepted: {
            playlist.addItems(fileDialog.fileUrls)
            fileDialog.visible = false
            if(!mediaPlayer.hasAudio)
                setAudioIndex(currentMusicIndex);
        }
    }

    Text{
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 25
        id: songTitle
        text: songNameAndAuthor()
        font.family: "Comic Sans"
        font.bold: true
        font.pointSize: 14
        color: "red"
    }

    DropShadow{
        anchors.fill: songTitle
        source: songTitle
        horizontalOffset: 3
        verticalOffset: 2
        radius: 1
        samples: 3
        color: "black"
    }

    function audioStatus(){
        musicPlaying = !musicPlaying
        if(musicPlaying){
            var currentMusicPosition = mediaPlayer.position
            mediaPlayer.seek(currentMusicPosition)
            mediaPlayer.play();
        }else
            mediaPlayer.pause();
    }

    function previousSong(){
        if(currentMusicIndex - 1 >= 0)
            currentMusicIndex--;
        setAudioIndex(currentMusicIndex)
    }

    Image{
        width: 54
        height: 54
        id: nextLeft
        source: "./content/images/previous.png"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width/2 - width - 150

        MouseArea{
            anchors.fill: parent
            onClicked: previousSong()
        }
    }

    Image{
        id: playButton
        width: 54
        height: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width/2 - width - 10
        source: musicPlaying ?  "./content/images/stop.png" : "./content/images/play.png"

        MouseArea{
            anchors.fill: parent
            onClicked:{
                if(mediaPlayer.hasAudio)
                    audioStatus()
            }
        }
    }

    Image{
        id: setMusic
        width: 54
        height: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width/2
        source: "./content/images/music.png"

        MouseArea{
            anchors.fill: parent
            onClicked:  fileDialog.visible = true
        }
    }

    function nextSong(){
        if(currentMusicIndex + 1 < playlist.itemCount)
            currentMusicIndex++;
        setAudioIndex(currentMusicIndex)
    }

    Image{
        width: 54
        height: 54
        id: nextRight
        source: "./content/images/next.png"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width/2 + width + 75

        MouseArea{
            anchors.fill: parent
            onClicked: nextSong()
        }
    }

    function formatVolumeToBetterRead(volume){
        var getVolume = volume
        getVolume *= 100
        getVolume = Math.floor(getVolume)
        getVolume = getVolume.toString() + "%"
        return getVolume
    }

    Slider{
        id: musicVolume
        value: mediaPlayer.volume
        width: 180
        tickmarksEnabled: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        onValueChanged: mediaPlayer.volume = musicVolume.value
        style: SliderStyle{
            groove: Rectangle{
                implicitHeight: 8
                implicitWidth: 180
                color: "gray"
                radius: 8
            }
            handle: Rectangle{
                anchors.centerIn: parent
                color: musicVolume.pressed ? "green" : "lightgray"
                implicitHeight: 24
                implicitWidth: 24
                radius: 12
                border.width: 1
                border.color: "gray"
            }
            tickmarks: Text{
                font.pointSize: 10
                y: parent.y - 12
                x: parent.x + 75
                color: "red"
                font.bold: true
                text: musicVolume.pressed ? formatVolumeToBetterRead(musicVolume.value) : ""
            }
        }
    }
}
