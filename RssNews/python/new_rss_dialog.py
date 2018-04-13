from PyQt5 import QtWidgets, QtGui, QtCore


class NewRss(QtWidgets.QDialog):
    def __init__(self, title="Add new rss",url="", category=""):
        QtWidgets.QDialog.__init__(self)

        self.setWindowTitle(title)

        self.rss_url = QtWidgets.QLineEdit(url)
        self.rss_url.setPlaceholderText("Feed Url")
        self.rss_category = QtWidgets.QLineEdit(category)
        self.rss_category.setPlaceholderText("Feed Category")

        main_layout = QtWidgets.QGridLayout()

        url_layout = QtWidgets.QHBoxLayout()
        url_layout.addWidget(self.rss_url)

        category_layout = QtWidgets.QHBoxLayout()
        category_layout.addWidget(self.rss_category)

        ok_button = QtWidgets.QPushButton("ok")
        cancel_button = QtWidgets.QPushButton("cancel")

        box_button = QtWidgets.QHBoxLayout()
        box_button.addWidget(ok_button)
        box_button.addWidget(cancel_button)

        main_layout.addLayout(url_layout, 0, 0)
        main_layout.addLayout(category_layout, 1, 0)
        main_layout.addLayout(box_button, 2, 0, QtCore.Qt.AlignRight)

        self.setLayout(main_layout)

        ok_button.clicked.connect(self.accept)
        cancel_button.clicked.connect(self.reject)

        self.setFixedWidth(420)
