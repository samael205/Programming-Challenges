# Description

A crawling spiders to searching web content and get specific data

## Usage

```python
from spider import AdafruitSpider, SearchingSpider

spider = AdafruitSpider()
data = spider.crawl()
spider.show(data)

spider = SearchingSpider()
spider.search("http://www.spacex.com", "rocket", 5, show=True)
```

