from PyQt5 import QtCore, QtWidgets, QtGui
import generator


class NameGenerator(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.setWindowTitle("Random Name Picker")
        self.setFocusPolicy(QtCore.Qt.StrongFocus)

        dimensions = QtWidgets.QApplication.desktop().screen()
        self.resize(530, 190)
        self.move(dimensions.rect().center() - self.rect().center())

        main_layout = QtWidgets.QVBoxLayout()

        self.generated_name = QtWidgets.QLineEdit()
        self.generated_name.setReadOnly(True)
        font = self.generated_name.font()
        font.setPointSize(31)
        self.generated_name.setFont(font)
        self.generated_name.setPlaceholderText("Click to generate")
        self.generated_name.setAlignment(QtCore.Qt.AlignCenter)
        self.generate_button = QtWidgets.QPushButton("Generate", self)
        font.setPointSize(15)
        self.generate_button.setFont(font)
        self.generate_button.setStyleSheet("padding: 15px")
        self.copy_to_clipboard_checkbox = QtWidgets.QCheckBox("Clipboard")

        box_layout = QtWidgets.QHBoxLayout()
        box_layout.addStretch(0)
        box_layout.addWidget(self.generate_button)
        box_layout.addWidget(self.copy_to_clipboard_checkbox)
        box_layout.addStretch(0)

        main_layout.addWidget(self.generated_name)
        main_layout.addLayout(box_layout)

        self.setLayout(main_layout)

        self.generate_button.clicked.connect(self.generate_name)

    def copy_to_clipboard(self):
        app = QtWidgets.QApplication
        data = QtCore.QMimeData()
        data.setText(self.generated_name.text())
        app.clipboard().setMimeData(data)

    def generate_name(self):
        random_name = generator.random_name_pick()
        self.generated_name.setText(random_name)

        if self.copy_to_clipboard_checkbox.isChecked():
            self.copy_to_clipboard()


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    icon = QtGui.QIcon()
    icon.addPixmap(QtGui.QPixmap("./icons/random.png"))
    app.setWindowIcon(icon)

    name_generator = NameGenerator()
    name_generator.show()

    sys.exit(app.exec_())
