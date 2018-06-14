from PyQt5 import QtCore, QtWidgets, QtGui, QtDataVisualization
from os import listdir, getcwd
from os.path import isfile, join
from time import sleep

import threading
import datetime
import analizer


class CustomFormatter(QtDataVisualization.QValue3DAxisFormatter):
    def __init__(self):
        QtDataVisualization.QValue3DAxisFormatter.__init__(self)

    def createNewInstance(self):
        return CustomFormatter()

    def populateCopy(self, copy):
        QtDataVisualization.QValue3DAxisFormatter.populateCopy(self, copy)

    def stringForValue(self, p_float, p_str):
        date = datetime.datetime.fromtimestamp(p_float)
        return date.strftime("%Y-%m-%d")


class MarketAnalizerWidget(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.time_distance = 31

        self.thread = QtCore.QThread()

        self.crypto_coins_list = QtWidgets.QListWidget()
        self.crypto_coins_list.setFixedWidth(100)
        self.crypto_coins_list.setDragEnabled(False)

        main_data_layout = QtWidgets.QHBoxLayout()
        main_data_layout.addWidget(self.crypto_coins_list)

        self.scatter = QtDataVisualization.Q3DScatter()
        container = QtWidgets.QWidget.createWindowContainer(self.scatter)

        proxy = QtDataVisualization.QScatterDataProxy()
        series = QtDataVisualization.QScatter3DSeries(proxy)
        series.setMeshSmooth(True)
        series.setItemSize(0.1)
        series.setItemLabelFormat("@xTitle: @xLabel @yTitle: @yLabel")

        self.scatter.addSeries(series)
        self.scatter.axisY().setTitle("Price (USD)")
        self.scatter.axisY().setTitleVisible(True)
        self.scatter.scene().activeCamera().setCameraPreset(QtDataVisualization.Q3DCamera.CameraPresetRightLow)
        self.scatter.setMargin(0.5)
        self.scatter.setShadowQuality(QtDataVisualization.QAbstract3DGraph.ShadowQualityNone)
        main_data_layout.addWidget(container)

        main_layout = QtWidgets.QGridLayout()
        main_layout.addLayout(main_data_layout, 0, 0)

        self.setup_scatter_formatter()

        self.current_main_crypto_prices = QtWidgets.QLabel()
        main_layout.addWidget(self.current_main_crypto_prices, 1, 0, QtCore.Qt.AlignCenter)

        self.setLayout(main_layout)
        self.setup_graph()
        self.setup_crypto_list()

        self.crypto_coins_list.itemSelectionChanged.connect(self.draw)
        self.crypto_coins_list.setCurrentRow(0)

        threading._start_new_thread(self.get_real_time_top_crypto_price, ())

    def setup_scatter_formatter(self):
        value_formatter_x = QtDataVisualization.QValue3DAxis()
        value_formatter_x.setFormatter(CustomFormatter())
        value_formatter_x.setTitle("Date")
        value_formatter_x.setTitleVisible(True)
        self.scatter.setAxisX(value_formatter_x)

        value_formatter_z = QtDataVisualization.QValue3DAxis()
        value_formatter_z.setFormatter(CustomFormatter())
        value_formatter_z.setTitleVisible(True)
        self.scatter.setAxisZ(value_formatter_z)

    def get_real_time_top_crypto_price(self):
        while True:
            try:
                btc = analizer.get_current_btc_price()
                eth = analizer.get_current_eth_price()
            except Exception as error:
                btc, eth = "nan", "nan"

            self.current_main_crypto_prices.setText("Real Time Price(USD) BTC: {}, ETH: {}".format(btc, eth))
            sleep(5)

    def setup_graph(self):
        self.scatter.activeTheme().setType(QtDataVisualization.Q3DTheme.ThemeArmyBlue)
        font = self.scatter.activeTheme().font()
        font.setPointSize(15)
        self.scatter.activeTheme().setFont(font)
        self.scatter.setShadowQuality(QtDataVisualization.QAbstract3DGraph.ShadowQualityMedium)

    def setup_crypto_list(self):
        current_dir = getcwd() + "/icons"
        icons = [icon for icon in listdir(current_dir) if isfile(join(current_dir, icon))]

        for icon in icons:
            icon_path = current_dir + "/" + icon
            self.crypto_coins_list.addItem(QtWidgets.QListWidgetItem(QtGui.QIcon(icon_path),
                                                                     icon.split(".")[0].upper()))

    def add_data(self, data):
        data_array = []
        data = data.tail(self.time_distance)
        for date, values in zip(data.index, data.values):
            data_array.append(QtDataVisualization.QScatterDataItem(QtGui.QVector3D(date.timestamp(),
                                              float(values),
                                              date.timestamp())))

        self.scatter.seriesList()[0].dataProxy().resetArray(data_array)

    def draw(self):
        row = self.crypto_coins_list.currentRow()
        item = self.crypto_coins_list.item(row)
        coin_code = item.text()
        self.add_data(analizer.cryptocurriencies[coin_code])


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        self.setWindowTitle("Cryptocurrency Market Analizer")

        self.main_widget = MarketAnalizerWidget()
        self.setCentralWidget(self.main_widget)

        self.resize(1200, 600)
        self.setup_menu()

    def setup_menu(self):
        menu = self.menuBar()
        menu_action = QtWidgets.QActionGroup(self)

        file = menu.addMenu("file")
        exit = file.addAction("exit")
        exit.setShortcut("CTRL+Q")
        exit.triggered.connect(QtWidgets.QApplication.exit)
        file.addAction(exit)

        filter = menu.addMenu("time")
        week = filter.addAction("week")
        week.setCheckable(True)
        week.setShortcut("CTRL+W")
        week.triggered.connect(self.week_filter)
        month = filter.addAction("month")
        month.setCheckable(True)
        month.setChecked(True)
        month.setShortcut("CTRL+M")
        month.triggered.connect(self.month_filter)

        menu_action.addAction(week)
        menu_action.addAction(month)
        menu_action.setExclusive(True)

    def reload_data(self):
        self.main_widget.draw()

    def week_filter(self):
        self.main_widget.time_distance = 8
        self.reload_data()

    def month_filter(self):
        self.main_widget.time_distance = 31
        self.reload_data()


if __name__ == '__main__':
    import sys

    app = QtWidgets.QApplication(sys.argv)
    app.setWindowIcon(QtGui.QIcon('./crypto.png'))

    cryptocurrency_monitor = MainWindow()
    cryptocurrency_monitor.show()

    sys.exit(app.exec_())
