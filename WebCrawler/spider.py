from bs4 import BeautifulSoup
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
            price = products[i].find('span', itemprop='price')
            product = products[i].h1
            data.append((product.text, price.text))
        return data

    def show(self, data):
        for product, price in data:
            print(product, ': $' + price)