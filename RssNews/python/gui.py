from PyQt5 import QtCore, QtWidgets, QtGui
from rss import rss, rss_contents, set_rss
from new_rss_dialog import NewRss
from pickle import dump


class RSSModel(QtCore.QAbstractTableModel):
    def __init__(self, data, parent=None, *args):
        super(RSSModel, self).__init__()
        self.blockSignals(True)
        self.data = data

    def rowCount(self, parent=QtCore.QModelIndex(), *args, **kwargs):
        return len(self.data)

    def columnCount(self, parent=QtCore.QModelIndex(), *args, **kwargs):
        return 1

    def update(self, new_data):
        self.data = new_data

    def headerData(self, p_int, orientation, role=None):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return QtCore.QVariant("Title")
        else:
            return QtCore.QVariant()

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if role == QtCore.Qt.DisplayRole:
            row = index.row()
            item = str(self.data[row])
            return item
        else:
            return QtCore.QVariant()

    def get_title(self, index):
        if not index.isValid:
            return QtCore.QVariant()

        row = index.row()
        title = str(self.data[row])
        return title

    def flags(self, index):
        return QtCore.Qt.ItemIsEnabled


class RssWidget(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)

        self.categories = QtWidgets.QListWidget()
        self.categories.setFixedWidth(100)
        font = QtGui.QFont("Garamond")
        font.setPointSize(8)
        font.setBold(True)
        self.categories.setFont(font)

        self.get_rss_categories()

        keys = list(rss_contents.keys())
        self.data_table = RSSModel(list(rss_contents[keys[0]].keys()))

        self.description = QtWidgets.QTextBrowser()
        self.description.setOpenExternalLinks(True)
        self.description.setReadOnly(True)
        self.description.setPlaceholderText("RSS Description")
        self.description.setFontPointSize(15)
        self.description.setFont(QtGui.QFont("Helvetica"))
        self.description.setLineWidth(2)
        self.description.acceptRichText()
        content_layout = QtWidgets.QVBoxLayout()

        self.data_table_view = QtWidgets.QTableView()
        self.data_table_view.horizontalHeader().setStretchLastSection(True)
        self.data_table_view.setModel(self.data_table)
        self.data_table_view.clicked.connect(self.show_rss_description)

        content_layout.addWidget(self.data_table_view)
        content_layout.addWidget(self.description)

        main_layout = QtWidgets.QHBoxLayout()
        main_layout.addWidget(self.categories)
        main_layout.addLayout(content_layout)

        self.setLayout(main_layout)

    def show_rss_titles_from_categories(self, data):
        content = data.text()
        titles = list(rss_contents[content].keys())
        self.data_table.update(titles)
        self.description.clear()

        for i in range(self.data_table.rowCount(), 0, -1):
            self.data_table_view.selectRow(i)

    def show_rss_description(self, index):
        title = self.data_table.get_title(index)
        current_rss_dict = rss_contents[self.categories.currentItem().text()]
        if title not in current_rss_dict:
            self.description.setText("")
            return

        description = current_rss_dict[title]
        description.replace(".", ". ")
        self.description.setText(description)

    def get_rss_categories(self):
        for key in rss.keys():
            items = self.categories.findItems(key, QtCore.Qt.MatchExactly)
            if key not in [item.text() for item in items]:
                self.categories.addItem(key)
                self.categories.itemClicked.connect(self.show_rss_titles_from_categories)


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        self.resize(850, 450)
        self.setWindowTitle("Rss Reader")
        self.rss_widget = RssWidget()
        self.setCentralWidget(self.rss_widget)

        rss_menu = self.menuBar()
        menu = rss_menu.addMenu("RSS")
        add_rss = QtWidgets.QAction("New", self)
        add_rss.triggered.connect(self.add_new_rss)
        add_rss.setShortcut(QtCore.Qt.Key_New)
        menu.addAction(add_rss)
        export = QtWidgets.QAction("Export", self)
        export.setShortcut(QtCore.Qt.Key_Save)
        export.triggered.connect(self.export_rss)
        menu.addAction(export)
        edit = QtWidgets.QAction("Edit", self)
        edit.setShortcut(QtCore.Qt.Key_Tab)
        edit.triggered.connect(self.edit_current_rss)
        menu.addAction(edit)
        menu.addSeparator()
        exit = QtWidgets.QAction("Exit", self)
        exit.setShortcut(QtCore.Qt.Key_Exit)
        exit.triggered.connect(self.quit)
        menu.addAction(exit)

        category_menu = rss_menu.addMenu("Category")
        remove_category = QtWidgets.QAction("Delete", self)
        remove_category.setShortcut(QtCore.Qt.Key_Delete)
        remove_category.triggered.connect(self.delete_category)
        category_menu.addAction(remove_category)

    def export_rss(self):
        file_path = QtWidgets.QFileDialog.getSaveFileName(self, "Export RSS")

        if not file_path[0]:
            return

        save_rss = open(file_path[0], "wb")
        dump(rss, save_rss)
        save_rss.close()

        self.statusBar().showMessage("Saved as " + file_path[0])

    def delete_category(self):
        selected = self.rss_widget.categories.selectedItems()

        if not selected:
            return    

        rss.pop(selected[0].text(), None)

        for item in selected:
            self.rss_widget.categories.takeItem(self.rss_widget.categories.row(item))

    def quit(self):
        QtWidgets.QApplication.quit()

    def edit_current_rss(self):
        current_rss = self.rss_widget.categories.currentItem().text()

        edit_rss = NewRss("Edit RSS", rss[current_rss], current_rss)

        if edit_rss.exec_():
            if current_rss == edit_rss.rss_category.text():
                return   

            rss.pop(current_rss, None)
            self.delete_category() 
            rss[edit_rss.rss_category.text()] = edit_rss.rss_url.text()
            self.rss_widget.get_rss_categories()
            set_rss()

    def add_new_rss(self):
        new_rss_dialog = NewRss()
        if new_rss_dialog.exec_():
            url = new_rss_dialog.rss_url.text()
            category = new_rss_dialog.rss_category.text()

            if not url or not category:
                return

            if "https://" not in url:
                url = "https://" + url

            if url not in rss.keys() and category not in rss.keys():
                rss[category] = url
                self.rss_widget.get_rss_categories()
                set_rss()

if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    icon = QtGui.QIcon()
    icon.addPixmap(QtGui.QPixmap("./icons/rss.png"))
    app.setWindowIcon(icon)

    reader = MainWindow()
    reader.show()

    sys.exit(app.exec_())