from PyQt5 import QtWidgets, QtCore


class IRCDialog(QtWidgets.QDialog):
    def __init__(self, title="Add new channel", server="", channel="", botnick=""):
        QtWidgets.QDialog.__init__(self)
        self.setWindowTitle(title)
        self.resize(250, 155)
        self.setFixedHeight(155)

        self.server = QtWidgets.QLineEdit()
        self.server.setText(server)
        self.server.setPlaceholderText("Server")

        self.channel = QtWidgets.QLineEdit()
        self.channel.setText(channel)
        self.channel.setPlaceholderText("Channel")

        self.nickname = QtWidgets.QLineEdit()
        self.nickname.setText(botnick)
        self.nickname.setPlaceholderText("Nickname")

        main_layout = QtWidgets.QGridLayout(self)

        data_layout = QtWidgets.QVBoxLayout()
        data_layout.addWidget(self.server)
        data_layout.addWidget(self.channel)
        data_layout.addWidget(self.nickname)

        ok_button = QtWidgets.QPushButton("ok")
        cancel_button = QtWidgets.QPushButton("canncel")

        button_box = QtWidgets.QHBoxLayout()
        button_box.addWidget(ok_button)
        button_box.addWidget(cancel_button)

        main_layout.addLayout(data_layout, 0, 0, QtCore.Qt.AlignCenter)
        main_layout.addLayout(button_box, 1, 0, QtCore.Qt.AlignRight)

        ok_button.clicked.connect(self.accept)
        cancel_button.clicked.connect(self.reject)

        self.setLayout(main_layout)
