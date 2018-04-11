import feedparser
import html


def parseRSS(feed_url):
    return feedparser.parse(feed_url)


def get_data(feed_url):
    data = dict()

    feed = parseRSS(feed_url)
    for item in feed["items"]:
        data[item["title"]] = html.unescape(item["description"])

    return data
