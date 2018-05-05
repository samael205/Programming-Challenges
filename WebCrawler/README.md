# Description

Spider which search all tags with specific text

# Python

import spider module and use it

```python
>>> import spider
>>> web_searcher = spider.SearchingSpider()
```

Search url to find tags with text and show their content and number of found tags

```python
>>> web_searcher.search_word("https://www.space.com/news", "rocket", 6)
1 https://www.space.com/news
> NASA's InSight Mars lander launched today (May 5) atop an Atlas V rocket, rising off a pad at California's Vandenberg Air Force Base at 7:05 a.m. EDT. InSight is the first-ever interplanetary mission to launch from the U.S. West Coast.
2 https://www.space.com/news
> NASA's InSight Mars lander launched today (May 5) atop an Atlas V rocket, rising off a pad at California's Vandenberg Air Force Base at 7:05 a.m. EDT. InSight is the first-ever interplanetary mission to launch from the U.S. West Coast.
3 https://www.space.com/tech-robots
> Planetary Resources' tiny Arkyd-6 satellite has completed all its mission goals in Earth orbit, just three months after lifting off atop an Indian rocket, company representatives said.
> The chief executive of Blue Origin says he expects the company's BE-4 engine to complete qualification testing by the end of the year as the company ramps up work on its New Glenn orbital rocket.
4 https://www.space.com/spaceflight
> var pageAccount = '/10518929/tmn.sdc';
    var pageAdZone = '/spaceflight';
    var siteName = 'tmn.sdc';
    var ccat = 'spaceflight';
    var ccatPretty = '';
    var ctype = '';
    var keywords = ["space\x20flightspace\x20shuttle\x20launchmission\x20\x26\x20rocket\x20launchesspaceflightspace\x20stationsatellites"];
    var ooValue = "0";
> NASA's InSight Mars lander launched today (May 5) atop an Atlas V rocket, rising off a pad at California's Vandenberg Air Force Base at 7:05 a.m. EDT. InSight is the first-ever interplanetary mission to launch from the U.S. West Coast.
5 https://www.space.com/science-astronomy
6 https://www.space.com/search-for-life

Tags: 6
```

