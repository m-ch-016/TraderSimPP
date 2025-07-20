import yfinance as yf
import csv
import random
import pandas as pd

ticker = "AAPL"
interval = "1m"
period = '5d'
outputFile = "data/orders.csv"

data = yf.download(ticker, interval=interval, period=period)

with open(outputFile, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Side", "Price", "Quantity"])

    for _, row in data.iterrows():
        try:
            volume = row["Volume"].item()
            low = row["Low"].item()
            high = row["High"].item()
        except (KeyError, ValueError, TypeError):
            continue

        if pd.isna(volume) or pd.isna(low) or pd.isna(high) or volume == 0 or low == 0 or high == 0:
            continue

        buy_qty = max(1, int(volume * random.uniform(0.2, 0.4) / 1000))
        sell_qty = max(1, int(volume * random.uniform(0.2, 0.4) / 1000))

        buy_price = round(low + random.uniform(0, 0.05), 2)
        sell_price = round(high - random.uniform(0, 0.05), 2)

        writer.writerow(["BUY", buy_price, buy_qty])
        writer.writerow(["SELL", sell_price, sell_qty])