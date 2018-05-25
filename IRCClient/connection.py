from irc import IRC
from PyQt5 import QtCore, QtWidgets, QtGui
from irc_dialog import IRCDialog
from pickle import load, dump
from os.path import isfile
import threading
import select
import atexit

mutex = QtCore.QMutex()


class IRCWidget(QtWidgets.QWidget):
    refresh_text = QtCore.pyqtSignal()

    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        data_layout = QtWidgets.QHBoxLayout()

        self.refresh_text.connect(self.refresh_data)

        self.connections = []

        self.channels = QtWidgets.QListWidget()
        self.channels.setFocusPolicy(QtCore.Qt.NoFocus)
        self.channels.setDragEnabled(False)
        self.channels.setFixedWidth(200)

        self.users_list = QtWidgets.QListWidget()
        self.users_list.setFocusPolicy(QtCore.Qt.NoFocus)
        self.users_list.setFixedWidth(150)

        if isfile("./settings"):
           self.load_settings()

        font = QtGui.QFont("Comic Sans")
        font.setPointSize(8)
        self.channels.setFont(font)

        self.irc_text_data = QtWidgets.QTextEdit()
        self.irc_text_data.setFocusPolicy(QtCore.Qt.NoFocus)
        self.irc_text_data.setReadOnly(True)

        data_layout.addWidget(self.channels)
        data_layout.addWidget(self.irc_text_data)
        data_layout.addWidget(self.users_list)

        self.send_message_line = QtWidgets.QLineEdit()
        self.send_message_line.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.send_message_line.setPlaceholderText("Send Message")

        main_layout = QtWidgets.QVBoxLayout()
        main_layout.addLayout(data_layout)
        main_layout.addWidget(self.send_message_line)

        self.setLayout(main_layout)

        self.send_message_line.returnPressed.connect(self.send_message)
        self.channels.clicked.connect(self.refresh_data)

        threading._start_new_thread(self.hold_connections, ())
        atexit.register(self.save_settings)

    def save_settings(self):
        for connection in self.connections:
            connection.irc_sock.close()

        settings = [[setting.server, setting.channel, setting.botnick] for setting in self.connections]

        with open("settings", "wb") as save_settings:
            dump(settings, save_settings)

    def load_settings(self):
        file = open("settings", "rb")
        saved_data = load(file)
        file.close()
        for data in saved_data:
            server, channel, botnick = data[0], data[1], data[2]
            new_irc = IRC(server, channel, botnick)
            self.new_channel(new_irc)

    def new_channel(self, irc):
        if irc not in self.connections:
            irc.connect()
            irc.join_channel()
            self.connections.append(irc)
            channel_item = irc.channel + "/" + irc.server
            self.channels.addItem(QtWidgets.QListWidgetItem(channel_item))
            self.channels.setCurrentRow(self.channels.count() - 1)

    def remove_channel(self):
        connection = self.current_connection()
        try:
            connection.disconnect()
        except Exception:
            pass

        self.connections.remove(connection)
        selected_items = self.channels.selectedItems()

        for item in selected_items:
            self.channels.takeItem(self.channels.row(item))

        self.refresh_data() if self.channels else self.irc_text_data.clear()

        if not self.channels:
            self.users_list.clear()

    def is_connections(self):
        if self.connections:
            return True
        else:
            return False

    def send_message(self):
        message = self.send_message_line.text()
        self.send_message_line.clear()

        if self.channels.count() == 0 or not message:
            return

        connection = self.current_connection()
        try:
            connection.send_message(message)
            self.current_connection().transcript += "\n" + "<" + connection.botnick + "> " + message
            self.refresh_text.emit()
        except Exception:
            self.irc_text_data.setTextColor(QtCore.Qt.red)
            self.irc_text_data.setText("Not connected to channel!")
            self.irc_text_data.setTextColor(QtCore.Qt.white)

    def show_users_in_channel(self):
        users = self.current_connection().users
        self.users_list.clear()
        if users:
            for user in users:
                self.users_list.addItem(QtWidgets.QListWidgetItem(user))

    def refresh_data(self):
        mutex.lock()
        self.irc_text_data.setText(self.current_connection().transcript)
        self.move_to_end_text_display()
        self.show_users_in_channel()
        mutex.unlock()

    def move_to_end_text_display(self):
        bar = self.irc_text_data.verticalScrollBar()
        bar.setValue(bar.maximum())

    def current_channel(self):
        return self.channels.currentItem().text()

    def current_connection(self):
        get_data = self.current_channel().split("/")
        server = get_data[1]
        channel = get_data[0]
        for connection in self.connections:
            if server == connection.server and channel == connection.channel:
                return connection

    def connect_to_current_channel(self):
        connection = self.current_connection()
        connection.connect()

    def disconnect_from_current_channel(self):
        connection = self.current_connection()
        try:
            connection.disconnect()
            connection.transcript = "\0"
        except Exception:
            self.irc_text_data.setTextColor(QtCore.Qt.red)
            self.irc_text_data.setText("Already disconnected from channel!")
            self.irc_text_data.setTextColor(QtCore.Qt.white)

    def hold_connections(self):
        while 1:
            if not self.is_connections():
                continue

            socks = dict((connection.irc_sock, connection) for connection in self.connections)
            ready_socks, _, _ = select.select(socks.keys(), [], [], 5)

            for sock in ready_socks:

                try:
                    data = socks[sock].receive_messages()
                except Exception:
                    pass

                if not data:
                    continue

                for connect in self.connections:
                    if connect.irc_sock == sock:
                        connect.transcript += "\n" + data
                        self.refresh_text.emit()


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        self.animation = QtCore.QPropertyAnimation(self, b"size")
        self.animation.setDuration(350)
        self.animation.setStartValue(QtCore.QSize(0, 0))
        self.animation.setEndValue(QtCore.QSize(1150, 750))
        self.animation.start()

        self.setWindowTitle("IRC Client")

        self.irc_widget = IRCWidget()
        self.setCentralWidget(self.irc_widget)

        self.connect_to_channel = QtWidgets.QAction("Connect", self)
        self.disconnect_from_channel = QtWidgets.QAction("Disconnect", self)
        self.remove_channel = QtWidgets.QAction("Remove", self)
        self.edit_channel = QtWidgets.QAction("Edit", self)
        self.setup_menu()

    def setup_menu(self):
        irc_menu = self.menuBar()
        
        channel_menu = irc_menu.addMenu("Channel")
        add_channel = QtWidgets.QAction("New", self)
        add_channel.triggered.connect(self.add_new_channel)
        add_channel.setShortcut("CTRL+N")
        channel_menu.addAction(add_channel)

        self.edit_channel.triggered.connect(self.edit_current_channel)
        self.edit_channel.setShortcut("CTRL+E")
        channel_menu.addAction(self.edit_channel)

        channel_menu.addSeparator()

        self.connect_to_channel.triggered.connect(self.connect)
        self.connect_to_channel.setShortcut("CTRL+C")
        channel_menu.addAction(self.connect_to_channel)

        self.disconnect_from_channel.triggered.connect(self.disconnect)
        self.disconnect_from_channel.setShortcut("CTRL+D")
        channel_menu.addAction(self.disconnect_from_channel)

        self.remove_channel.triggered.connect(self.remove)
        self.remove_channel.setShortcut("CTRL+R")
        channel_menu.addAction(self.remove_channel)

        channel_menu.addSeparator()

        exit_program = QtWidgets.QAction("Exit", self)
        exit_program.triggered.connect(QtWidgets.QApplication.quit)
        exit_program.setShortcut("CTRL+Q")
        channel_menu.addAction(exit_program)

        self.update_actions()

    def update_actions(self):
        self.connect_to_channel.setEnabled(self.irc_widget.is_connections())
        self.disconnect_from_channel.setEnabled(self.irc_widget.is_connections())
        self.remove_channel.setEnabled(self.irc_widget.is_connections())
        self.edit_channel.setEnabled(self.irc_widget.is_connections())

    def connect(self):
        self.irc_widget.connect_to_current_channel()
        self.update_actions()

    def disconnect(self):
        self.irc_widget.disconnect_from_current_channel()
        self.update_actions()

    def remove(self):
        self.irc_widget.remove_channel()
        self.update_actions()

    def add_new_channel(self):
        new_channel_dialog = IRCDialog()

        if new_channel_dialog.exec_():
            server = new_channel_dialog.server.text()
            channel = new_channel_dialog.channel.text()
            nickname = new_channel_dialog.nickname.text()

            if not server or not channel or not nickname:
                return

            new_irc = IRC(server, channel, nickname)
            self.irc_widget.new_channel(new_irc)
            self.update_actions()

    def edit_current_channel(self):
        current_connection = self.irc_widget.current_connection()
        edit_channel_dialog = IRCDialog("Edit channel",
                                            current_connection.server,
                                            current_connection.channel,
                                            current_connection.botnick)

        if edit_channel_dialog.exec_():
            server = edit_channel_dialog.server.text()
            channel = edit_channel_dialog.channel.text()
            botnick = edit_channel_dialog.nickname.text()

            if not server or not channel or not botnick\
                    or server == current_connection.server\
                    and channel == current_connection.channel\
                    and botnick == current_connection.botnick:
                return

            self.irc_widget.remove_channel()
            edited_irc = IRC(server, channel, botnick)
            self.irc_widget.new_channel(edited_irc)


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    icon = QtGui.QIcon("./icons/irc.png")
    app.setWindowIcon(icon)

    irc_client = MainWindow()
    irc_client.show()

    sys.exit(app.exec_())
