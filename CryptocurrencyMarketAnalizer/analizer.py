import pickle
import pandas as pd
import datetime
import quandl
import numpy as np
import urllib.request, json

from os import system

system("python settings.py")


def get_quandl_data(quandl_id):
    cache_path = "{}.pkl".format(quandl_id).replace("/", "-")
    try:
        file = open(cache_path, "rb")
        data_frame = pickle.load(file)
    except (OSError, IOError) as error:
        data_frame = quandl.get(quandl_id, returns="pandas")
        data_frame.to_pickle(cache_path)
    return data_frame


btc_usd_price_kraken = get_quandl_data("BCHARTS/KRAKENUSD")


def get_json_data(json_url, cache_path):
    try:
        file = open(cache_path, "rb")
        data_frame = pickle.load(file)
    except (OSError, IOError) as error:
        data_frame = pd.read_json(json_url)
        data_frame.to_pickle(cache_path)
    return data_frame


exchanges = ["COINBASE", "BITSTAMP", "ITBIT"]
exchange_data = dict()
exchange_data["KRAKEN"] = btc_usd_price_kraken

for exchange in exchanges:
    exchange_code = "BCHARTS/{}USD".format(exchange)
    btc_exchange_data_frame =  get_quandl_data(exchange_code)
    exchange_data[exchange] = btc_exchange_data_frame


def merge_dfs_on_column(data_frames, labels, col):
    series_dict = dict()
    for index in range(len(data_frames)):
        series_dict[labels[index]] = data_frames[index][col]
    return pd.DataFrame(series_dict)


btc_usd_datasets = merge_dfs_on_column(list(exchange_data.values()), list(exchange_data.keys()), "Weighted Price")
btc_usd_datasets.replace(0, np.nan, inplace=True)
btc_usd_datasets['avg_btc_price_usd'] = btc_usd_datasets.mean(axis=1)

main_data_url = 'https://poloniex.com/public?command=returnChartData&currencyPair={}&start={}&end={}&period={}'
start_date = datetime.datetime.today() - datetime.timedelta(93)
end_date = datetime.datetime.now()
pedoid = 86400


def get_crypto_data(poloniex_pair):
    json_url = main_data_url.format(poloniex_pair, start_date.strftime("%s"), end_date.timestamp(), pedoid)
    data_frame = get_json_data(json_url, poloniex_pair)
    data_frame = data_frame.set_index('date')
    return data_frame


coins = ["ETH", "LTC", "XRP", "ETC", "DASH", "XMR", "XEM", "LSK", "DCR", "MAID", "CLAM"]
coins_data = dict()

for coin in coins:
    coin_pair = 'BTC_{}'.format(coin)
    crypto_price_data_frame = get_crypto_data(coin_pair)
    coins_data[coin] = crypto_price_data_frame

for coin in coins_data.keys():
    coins_data[coin]['price_usd'] = coins_data[coin]['weightedAverage'] * btc_usd_datasets['avg_btc_price_usd']


cryptocurriencies = merge_dfs_on_column(list(coins_data.values()), list(coins_data.keys()), "price_usd")
cryptocurriencies["BTC"] = btc_usd_datasets['avg_btc_price_usd']


def format_value(value):
    return float("{0:.3f}".format(value))


def get_current_btc_price():
    with urllib.request.urlopen('https://blockchain.info/ticker') as url:
        data = json.loads(url.read().decode())
        return format_value(float(data['USD']['last']))


def get_current_eth_price():
    with urllib.request.urlopen('https://api.coinmarketcap.com/v1/ticker/ethereum/') as url:
        data = json.loads(url.read().decode())
        return format_value(float(data[0]['price_usd']))
