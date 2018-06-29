# Description

A crawling spiders which get a needed data from specific website

## Usage

```python
from spider import AdafruitSpider, SearchingSpider

spider = AdafruitSpider()
data = spider.crawl()
spider.show(data)

spider = SearchingSpider()
spider.search("http://www.spacex.com", "rocket", 5, show=True)
```

