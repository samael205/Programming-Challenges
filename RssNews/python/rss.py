import reader
import atexit
from pickle import dump, load
from os.path import isfile
from os import remove

if not isfile("./rss"):
    rss = {
        "Hacker News": "https://news.ycombinator.com/rss",
        "Japan": 'https://www.japantimes.co.jp/news/rss'
    }
else:
    file = open("rss", "rb")
    rss = load(file)
    file.close()

rss_contents = dict()


def set_rss():
    for key in rss.keys():
        rss_data = reader.get_data(rss[key])
        rss_contents[key] = rss_data

set_rss()


def save():
    if not rss:
        remove("rss")
        return

    save_rss = open("rss", "wb")
    save_rss.truncate()
    dump(rss, save_rss)
    save_rss.close()


atexit.register(save)