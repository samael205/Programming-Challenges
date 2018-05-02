# Description

Spider which search all tags with specific text

# Python

import spider module and use it

```python
>>> import spider
>>> web_searcher = spider.SearchingSpider()
```

Search url to find tags with text and show their content

```python
>>> web_searcher.search_word("http://www.spacex.com/", "Rocket", 8)
1 http://www.spacex.com/
> Rockets & Spacecraft
> Rocket Road
2 http://www.spacex.com/#main-menu
> Rockets & Spacecraft
> Rocket Road
3 http://www.spacex.com/
> Rockets & Spacecraft
> Rocket Road
4 http://www.spacex.com/falcon9
> Rocket pieces
> Rockets & Spacecraft
> Rocket Road
5 http://www.spacex.com/falcon-heavy
> Most Powerful Rocket
> The World's Most Powerful Rocket
> Rockets & Spacecraft
> Rocket Road
6 http://www.spacex.com/dragon
> Rockets & Spacecraft
> Rocket Road
7 http://www.spacex.com/news
> World's First Reflight of an Orbital Class Rocket
> World's First Reflight of an Orbital Class Rocket
> World's First Reflight of an Orbital Class Rocket
> Rockets & Spacecraft
> Rocket Road
8 http://www.spacex.com/about
> Transforming the Way Rockets Are Made
> Rocket Road
> Rockets & Spacecraft
> Rocket Road
```

