import json
import os

from os.path import expanduser


def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)


def active_tabs():
    if os.path.exists(expanduser("~/.mozilla/firefox")):
            sessions = open(find("recovery.js", expanduser("~/.mozilla/firefox")), "r")
    jdata = json.loads(sessions.read())
    sessions.close()
    tabs = list()
    for win in jdata.get("windows"):
        for tab in win.get("tabs"):
            i = tab.get("index")-1
            strUrl = tab.get("entries")[i].get("url")
            if "www.youtube.com" in strUrl:
                tabs.append(strUrl)
    return tabs
