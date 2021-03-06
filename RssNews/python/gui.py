from PyQt5 import QtCore, QtWidgets, QtGui
from PyQt5.QtWebEngineWidgets import QWebEngineView
from feeds import rss, rss_contents, set_rss
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
        if role == QtCore.Qt.DisplayRole and index.isValid:
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
        if keys:
            self.data_table = RSSModel(list(rss_contents[keys[0]].keys()))
        else:
            self.data_table = RSSModel([])   

        self.description = QWebEngineView()
        self.description.setFont(QtGui.QFont("Helvetica"))
        self.description.setFocusPolicy(QtCore.Qt.NoFocus)

        content_layout = QtWidgets.QVBoxLayout()

        self.data_table_view = QtWidgets.QTableView()
        self.data_table_view.horizontalHeader().setStretchLastSection(True)
        self.data_table_view.setModel(self.data_table)
        x = self.frameGeometry().height()
        self.data_table_view.setFixedHeight(x/2-10)
        self.data_table_view.clicked.connect(self.show_rss_description)

        content_layout.addWidget(self.data_table_view)
        content_layout.addWidget(self.description)

        main_layout = QtWidgets.QHBoxLayout()
        main_layout.addWidget(self.categories)
        main_layout.addLayout(content_layout)

        self.setLayout(main_layout)

    def is_categories(self):
        if self.categories.count() > 0:
            return True
        else:
            return False

    def refresh_table(self):
        self.description.setHtml("")
        for i in range(self.data_table.rowCount(), 0, -1):
            self.data_table_view.selectRow(i)    
            
    def show_rss_titles_from_categories(self, data):
        if data is None:
            self.data_table.update([])
            return

        content = data.text()
        titles = list(rss_contents[content].keys())

        self.data_table.update(titles)

        self.refresh_table()

    def show_rss_description(self, index):
        title = self.data_table.get_title(index)
        current_rss_dict = rss_contents[self.categories.currentItem().text()]
        if title not in current_rss_dict:
            self.description.setHtml("")
            return

        description = current_rss_dict[title]
        self.description.setHtml(description)

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
   
        self.edit = QtWidgets.QAction("Edit", self)
        self.remove_category = QtWidgets.QAction("Delete", self)
        self.export = QtWidgets.QAction("Export", self)
        self.setup_menu()

    def setup_menu(self):
        rss_menu = self.menuBar()
        
        menu = rss_menu.addMenu("RSS")
        add_rss = QtWidgets.QAction("New", self)
        add_rss.triggered.connect(self.add_new_rss)
        add_rss.setShortcut("CTRL+N")
        menu.addAction(add_rss)
            
        self.export.triggered.connect(self.export_rss)
        self.export.setShortcut("CTRL+S")
        menu.addAction(self.export)
     
        self.edit.triggered.connect(self.edit_current_rss)
        self.edit.setShortcut("CTRL+E")
        menu.addAction(self.edit)
        
        menu.addSeparator()
        
        exit = QtWidgets.QAction("Exit", self)
        exit.setShortcut(QtCore.Qt.Key_Exit)
        exit.triggered.connect(self.quit)
        menu.addAction(exit)

        category_menu = rss_menu.addMenu("Category")
        self.remove_category.setShortcut(QtCore.Qt.Key_Delete)
        self.remove_category.triggered.connect(self.delete_category)
        category_menu.addAction(self.remove_category)
      
    def update_actions(self):
        self.edit.setEnabled(self.rss_widget.is_categories())
        self.remove_category.setEnabled(self.rss_widget.is_categories())
        self.export.setEnabled(self.rss_widget.is_categories())

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

        confirm_delete = QtWidgets.QMessageBox()   
        confirm_delete.setText("Delete " + selected[0].text() + "?")
        confirm_delete.setWindowTitle("Remove RSS")
        confirm_delete.setStandardButtons(QtWidgets.QMessageBox.Ok | QtWidgets.QMessageBox.Cancel)
        
        decision = confirm_delete.exec_()

        if decision == QtWidgets.QMessageBox.Ok:
            rss.pop(selected[0].text(), None)
        
            for item in selected:
                self.rss_widget.categories.takeItem(self.rss_widget.categories.row(item))

            self.update_actions()

            if self.rss_widget.categories.count != 0:    
                self.rss_widget.categories.setCurrentRow(0)  
                self.rss_widget.show_rss_titles_from_categories(self.rss_widget.categories.item(0)) 
            
    def quit(self):
        QtWidgets.QApplication.quit()

    def edit_current_rss(self):
        current_rss = self.rss_widget.categories.currentItem().text()

        edit_rss = NewRss("Edit RSS", rss[current_rss], current_rss)

        if edit_rss.exec_():
            if current_rss == edit_rss.rss_category.text():
                return   

            rss.pop(current_rss, None)
            
            selected = self.rss_widget.categories.selectedItems()
            self.rss_widget.categories.takeItem(self.rss_widget.categories.row(selected[0]))
 
            rss[edit_rss.rss_category.text()] = edit_rss.rss_url.text()
            self.rss_widget.get_rss_categories()
            set_rss()

            edited_item = self.rss_widget.categories.findItems(edit_rss.rss_category.text(), QtCore.Qt.MatchExactly)
            item_row = self.rss_widget.categories.row(edited_item[0])
            self.rss_widget.categories.setCurrentRow(item_row)

    def add_new_rss(self):
        new_rss_dialog = NewRss()
        if new_rss_dialog.exec_():
            url = new_rss_dialog.rss_url.text()
            category = new_rss_dialog.rss_category.text()

            if not url or not category:
                return

            if url not in rss.keys() and category not in rss.keys():
                rss[category] = url
                self.rss_widget.get_rss_categories()
                set_rss()
                self.update_actions()

if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    icon = QtGui.QIcon()
    icon.addPixmap(QtGui.QPixmap("./icons/rss.png"))
    app.setWindowIcon(icon)

    reader = MainWindow()
    reader.show()

    sys.exit(app.exec_())
