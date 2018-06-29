from bs4 import BeautifulSoup
import re
import csv
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


class Spider:
    def __init__(self, url):
        self.start_url = url
        pass

    def open_url(self, url):
        http = urllib3.PoolManager()
        response = http.request('GET', url)
        return response


class AsianRestaurantSpider(Spider):
    def __init__(self, url="https://www.tripadvisor.com/Restaurants-g1066456-Shibuya_Tokyo_Tokyo_Prefecture_Kanto.html"):
        super().__init__(url)
        self.links = []

    def get_links(self):
        response = self.open_url(self.start_url)
        soup = BeautifulSoup(response.data, 'html.parser')
        links = soup.find_all('a', {'class': 'property_title'})
        main_url = self.start_url.split("R")[0]
        for link in links:
            href = link['href']
            self.links.append(main_url + href)

    def get_restaurant_data(self, url):
        response = self.open_url(url)
        soup = BeautifulSoup(response.data, "html.parser")
        title = soup.find('h1', id='HEADING').text
        opinions = soup.find_all('span', {'class': 'noQuotes'})
        opinions = [opinion.text for opinion in opinions]
        return {title: opinions}

    def save_to_csv(self, name, data):
        with open(name, 'w') as file:
            writer = csv.writer(file)
            names, opinions = [], []
            for restaurants in data:
                for restaurant in restaurants:
                    names.append(restaurant)
                    opinions.append(restaurants[restaurant])
            writer.writerow(names)
            writer.writerows(zip(*opinions))

    def crawl(self, max=5):
        self.get_links()
        restaurant_data = []
        for link, i in zip(self.links, range(0, max)):
            data = self.get_restaurant_data(link)
            restaurant_data.append(data)
        return restaurant_data


class AdafruitSpider(Spider):
    def __init__(self, url="https://www.adafruit.com/new"):
        super().__init__(url)

    def crawl(self):
        response = self.open_url(self.start_url)
        soup = BeautifulSoup(response.data, 'html.parser')
        products = soup.find_all('div', {'class': 'product-listing-right'})
        data = []
        for i in range(0, len(products)):
            product = products[i].h1
            price = products[i].find('span', itemprop='price')    
            description = products[i].find('div', {'class': 'product-description'})	
            data.append((product.text, price.text, description.text))
        return data

    def show(self, data):
        for product, price, description in data:
            print("\033[95m{}\033[0m : $\033[93m{}\033[0m{}".format(product, price, description))


class AmazonElectronicsSpider(Spider):
    def __init__(self, url="https://www.amazon.com/Best-Sellers-Electronics/zgbs/"
                           "electronics/ref=zg_bs_pg_3?_encoding=UTF8&pg={}"):
        super().__init__(url)

    def crawl(self):
        data = []
        for page in range(1, 6):
            url = self.start_url.format(page)
            response = self.open_url(url)
            soup = BeautifulSoup(response.data, 'html.parser')
            products = soup.select(".zg_itemWrapper")
            for product in products:
                title = product.div.img['alt']
                price = product.find("span", {'class': 'p13n-sc-price'})
                price = price.text if price else 'NaN'
                rating = product.find("span", {'class': 'a-icon-alt'})
                rating = rating.text if rating else 'NaN'
                data.append((title, price, rating))
        return data

    def save_to_csv(self, name, data):
        with open(name, 'w') as file:
            writer = csv.writer(file)
            headers = ['Product', 'Price', 'Rating']
            writer.writerow(headers)
            for product, price, rating in data:
                writer.writerow([product, price, rating])

    def show(self, data):
        for product, price, rating in data:
            print("\033[94m{}\033[0m : \033[93m{}\033[0m\n{}\n".format(product, price, rating))


class SearchingSpider(Spider):
    def __init__(self):
        super().__init__(self)
        self.links = []

    def get_links(self, url):
        response = self.open_url(url)
        links = []
        soup = BeautifulSoup(response.data, 'html.parser')
        for link in soup.find_all('a', attrs={'href': re.compile("^http://")}):
            links.append(link['href'])
        return soup, links

    def search(self, start_url, content, max_pages=10, show=False):
        self.links = [start_url]
        page = 0
        founded = False

        while page < max_pages and self.links != []:
            page += 1
            current_url = self.links.pop(0)
            web_data, new_links = self.get_links(current_url)
            if content in web_data.text:
                founded = True
                print("\033[92mFound in\033[0m", current_url)
                if show:
                    data = web_data.find(text=re.compile(content)).split()
                    print('\033[91m>\033[0m', ' '.join(data))
            self.links.extend(new_links)

        if not founded:
            print("Not found")
