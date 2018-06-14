import glob, os, os.path
import datetime
import atexit


def erase_current_data():
    quandl_files = glob.glob(os.path.join(os.getcwd(), "*.pkl"))
    poloniex_files = glob.glob(os.path.join(os.getcwd(), "BTC_*"))
    all_files = quandl_files
    all_files.extend(poloniex_files)
    for file in all_files:
        os.remove(file)


with open("date", "r") as last_date:
    if last_date.read() != datetime.datetime.now().strftime("%Y-%m-%d") or not last_date:
        erase_current_data()


def save_current_date():
    with open("date", "w") as file:
        current_date = datetime.datetime.now().strftime("%Y-%m-%d")
        file.write(current_date)


atexit.register(save_current_date)