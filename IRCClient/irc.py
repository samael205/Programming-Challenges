import socket


class IRC:
    def __init__(self, server, channel, botnick):
        self.server = server
        self.irc_sock = None
        self.channel = channel
        self.botnick = botnick
        self.users = []
        self.transcript = ""

    def __eq__(self, other):
        return self.channel == other.channel and self.server == other.server

    def connect(self):
        self.irc_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.irc_sock.connect((self.server, 6667))
        self.irc_sock.send(bytes("USER " + self.botnick + " " + self.botnick + " " + self.botnick + " " + self.botnick + "\n", "UTF-8"))
        self.irc_sock.send(bytes("NICK " + self.botnick + "\n", "UTF-8"))

    def join_channel(self):
        self.irc_sock.send(bytes("JOIN " + self.channel + "\n", "UTF-8"))

    def ping(self):
        self.irc_sock.send(bytes("PONG :pingis\n", "UTF-8"))

    def send_message(self, message):
        self.irc_sock.send(bytes("PRIVMSG " + self.channel + " :" + message + "\n", "UTF-8"))

    def receive_messages(self):
        irc_message = self.irc_sock.recv(2048).decode("UTF-8")
        irc_message = irc_message.strip("\n\r")

        if irc_message.find("PING :") != -1:
            self.ping()

        if irc_message.find("End of /NAMES list.") != -1:
            users = irc_message[irc_message.find(":" + self.botnick)+1:]
            users = users[:users.find(":")]
            self.users = users.split()

        if irc_message.find("PRIVMSG") != -1:
            name = irc_message.split("!", 1)[0][1:]
            message = irc_message.split("PRIVMSG", 1)[1].split(":", 1)[1]
            return "<" + name + "> " + message

        return irc_message

    def disconnect(self):
        self.irc_sock.send(bytes("QUIT \n", "UTF-8"))
