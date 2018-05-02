from html.parser import HTMLParser
from urllib import parse
from urllib.request import urlopen
from bs4 import BeautifulSoup
import re


class SearchingSpider(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.links = []
        self.base_url = None

    def search_word(self, url, key_word, pages):
        links = [url]
        current_page_number = 0

        while current_page_number < pages:
            current_url = links[0]
            links.pop(0)

            current_page_number += 1

            print(current_page_number, current_url)
            web_data, new_links = self.get_links(current_url)

            if web_data.find(key_word) != -1:
                parsed_html = BeautifulSoup(web_data, "html.parser")
                tags_with_key_word = parsed_html.find_all(text=re.compile(key_word))
                for tag in tags_with_key_word:
                    if "http" not in tag:
                        content = tag
                        content = content.replace(key_word, "\033[10;31m" + key_word + "\033[m")
                        print("\033[00;35m>\033[m", content.strip())

            links.extend(new_links)

    def handle_starttag(self, tag, attrs):
        url_regex = re.compile(r'^(?:http|ftp)s?://', re.IGNORECASE)

        if tag.lower() == "a":
            for key, value in attrs:
                    if key == "href":
                        new_url = parse.urljoin(self.base_url, value)
                        if re.match(url_regex, new_url) is not None:
                            self.links.append(new_url)

    def get_links(self, url):
        self.base_url = url
        response = urlopen(self.base_url, timeout=10)
        html_info = response.info()
        content_type = html_info.get_content_type()

        if content_type == "text/html":
            get_web = response.read()
            html_content = get_web.decode("utf-8")
            self.feed(html_content)
            return html_content, self.links
        else:
            return "", []
