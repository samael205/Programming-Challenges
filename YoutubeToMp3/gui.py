import getUrls
import youtube_to_mp3
import time
from PyQt5 import QtGui, QtWidgets, QtCore


class progressThread(QtCore.QThread):
    progress_update = QtCore.pyqtSignal(int)
    download_items = QtCore.pyqtSignal(str)
    close = QtCore.pyqtSignal(str)
    data = list()
    current = 0

    def __init__(self):
        QtCore.QThread.__init__(self)

    def __del__(self):
        self.wait()

    def get_data(self, data):
        for i in data:
            self.data.append(i)
        self.download_items.emit(str(self.current) + "/" + str(len(self.data)))

    def no_history_duplicate(self):
        file = open("history.txt", "r+")
        new_content = list()
        for i in file.readlines():
            if i not in new_content:
                new_content.append(i)
        file.truncate()
        file.close()
        file = open("history.txt", "w")
        for i in new_content:
            file.write(i)
        file.close()

    def run(self):
        for i in self.data:
            youtube_to_mp3.download(i)
            self.current = self.current + 1
            self.download_items.emit(str(self.current) + "/" + str(len(self.data)))
            self.progress_update.emit(self.current)
            time.sleep(1)
        self.close.emit("finished")
        self.data.clear()
        self.current = 0
        self.no_history_duplicate()


class Downloader(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.setWindowTitle("Youtube mp3 Downloader")
        self.resize(330, 100)
        self.buttons = [QtWidgets.QPushButton("Active Tabs", self),
                        QtWidgets.QPushButton("Links", self),
                        QtWidgets.QPushButton("History", self),
                        QtWidgets.QPushButton("Download", self),
                        QtWidgets.QPushButton("Back", self)]
        self.getLinks = QtWidgets.QTextEdit()
        self.navigate = QtWidgets.QVBoxLayout()
        self.navigate.addWidget(self.buttons[3])
        self.navigate.addWidget(self.buttons[4])
        layout = QtWidgets.QGridLayout()
        layout.addWidget(self.buttons[0], 0, 0)
        layout.addWidget(self.buttons[1], 0, 1)
        layout.addWidget(self.buttons[2], 0, 2)
        layout.addLayout(self.navigate, 0, 1)
        layout.addWidget(self.getLinks, 0, 0)
        self.setLayout(layout)
        self.buttons[3].hide()
        self.buttons[4].hide()
        self.getLinks.hide()
        self.buttons[1].clicked.connect(self.DownloadLinksMenu)
        self.buttons[3].clicked.connect(self.DownloadLinks)
        self.buttons[4].clicked.connect(self.Back)
        self.buttons[2].clicked.connect(self.History)
        self.buttons[0].clicked.connect(self.DownloadTabs)
        tabs_icon = QtGui.QIcon("tabs.png")
        links_icon = QtGui.QIcon("url.png")
        history_icon = QtGui.QIcon("history.png")
        download_icon = QtGui.QIcon("download.png")
        back_icon = QtGui.QIcon("back.png")
        self.buttons[0].setIcon(tabs_icon)
        self.buttons[0].setStyleSheet("background-color:#718D96;")
        self.buttons[1].setIcon(links_icon)
        self.buttons[1].setStyleSheet("background-color:#718D96;")
        self.buttons[2].setIcon(history_icon)
        self.buttons[2].setStyleSheet("background-color:#718D96;")
        self.buttons[3].setIcon(download_icon)
        self.buttons[4].setIcon(back_icon)
        self.progress = QtWidgets.QDialog(self)
        self.progress.resize(260, 60)
        self.progress.setWindowTitle("Getting mp3")
        self.progressBar = QtWidgets.QProgressBar(self.progress)
        self.progressBar.setValue(0)
        self.progressBar.setTextVisible(True)
        self.progressBar.resize(260, 60)
        self.progressInfo = QtWidgets.QLabel(self.progress)
        self.progressInfo.move(45, 4)
        self.progressInfo.setText("")
        self.updateBar(0)
        self.process = progressThread()
        self.process.progress_update.connect(self.updateBar)
        self.process.download_items.connect(self.progressBar.setFormat)
        self.process.close.connect(self.exitBar)
        self.animation = QtCore.QPropertyAnimation(self, b"size")
        self.animation.setDuration(150)
        dimensions = QtWidgets.QApplication.desktop().screenGeometry()
        self.move(int(dimensions.width()/2)-100, int(dimensions.height()/2)-100)

    def DownloadLinks(self):
        if self.getLinks.toPlainText() != "":
            self.totalBar(len(self.getLinks.toPlainText().split("\n")))
            self.process.get_data(self.getLinks.toPlainText().split("\n"))
            self.progress.open()
            self.process.start()
            self.getLinks.clear()

    def DownloadLinksMenu(self):
        self.getLinks.setPlaceholderText("Insert Youtube URLs here")
        self.getLinks.clear()
        self.buttons[0].hide()
        self.buttons[1].hide()
        self.buttons[2].hide()
        self.buttons[3].show()
        self.buttons[4].show()
        self.getLinks.show()
        self.animation.setEndValue(QtCore.QSize(500, 130))
        self.animation.start()

    def DownloadTabs(self):
        self.totalBar(len(getUrls.active_tabs()))
        self.progress.open()
        self.process.get_data(getUrls.active_tabs())
        self.process.start()

    def History(self):
        self.getLinks.setReadOnly(True)
        self.getLinks.setPlaceholderText("Downloaded links")
        file = open("history.txt", 'r')
        links = file.readlines()
        file.close()
        str = ""
        for link in links:
            str += link
        self.getLinks.setText(str)
        self.buttons[0].hide()
        self.buttons[1].hide()
        self.buttons[2].hide()
        self.buttons[3].hide()
        self.buttons[4].show()
        self.getLinks.show()
        self.animation.setEndValue(QtCore.QSize(800, 200))
        self.animation.start()

    def Back(self):
        self.buttons[0].show()
        self.buttons[1].show()
        self.buttons[2].show()
        self.buttons[3].hide()
        self.buttons[4].hide()
        self.getLinks.hide()
        self.animation.setEndValue(QtCore.QSize(330, 100))
        self.animation.start()
        self.getLinks.setReadOnly(False)

    def exitBar(self):
        self.progress.hide()
        self.progressBar.setValue(0)

    def totalBar(self, n):
        self.progressBar.setMaximum(n)

    def updateBar(self, n):
        self.progressBar.setValue(n)


if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    download = Downloader()
    download.show()
    sys.exit(app.exec_())




