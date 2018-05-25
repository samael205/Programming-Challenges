import feedparser
import html


def parseRSS(feed_url):
    return feedparser.parse(feed_url)


def get_data(feed_url):
    data = dict()

    feed = parseRSS(feed_url)
    for item in feed["items"]:
        try:
            data[item["title"]] = html.unescape(item["description"])
        except Exception:
            data[item["title"]] = ""

    return data
