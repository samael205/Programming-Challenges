import QtQuick 2.0

ListModel{
    ListElement { name: "Hacker News"; feed: "news.ycombinator.com/rss"; image: "icons/hacker.png"}
    ListElement { name: "Science"; feed: "www.sciencedaily.com/rss/top/science.xml"; image: "icons/science.jpeg" }
    ListElement { name: "NASA"; feed: "www.nasa.gov/rss/dyn/breaking_news.rss"; image: "icons/space.jpeg"}
    ListElement { name: "Space"; feed: "www.space.com/home/feed/site.xml"; image: "icons/space.png"}
    ListElement { name: "Japan"; feed: "www.japantimes.co.jp/news/rss"; image: "icons/japan.png"}
    ListElement { name: "Technology"; feed: "www.sciencedaily.com/rss/top/technology.xml"; image: "icons/technology.png"}
}
