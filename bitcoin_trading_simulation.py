import yfinance as yf
import pandas as pd
import matplotlib.pyplot as plt

def run_simulation(symbol="BTC-USD", initial_balance=10000, short_window=20, long_window=50):
    print(f"--- Starting Simulation for {symbol} ---")
    
    # 1. Download Historical Data (Last 1 year, 1-day intervals)
    data = yf.download(symbol, period="1y", interval="1d")
    if data.empty:
        print("Failed to fetch data. Check your connection or symbol.")
        return

    # 2. Calculate Indicators (Simple Moving Averages)
    data['SMA20'] = data['Close'].rolling(window=short_window).mean()
    data['SMA50'] = data['Close'].rolling(window=long_window).mean()

    # 3. Simulation Variables
    balance = initial_balance  # USD
    btc_held = 0               # Amount of BTC owned
    state = "IDLE"             # Current position status
    history = []               # To track portfolio value over time

    # 4. Trading Loop
    for i in range(len(data)):
        price = float(data['Close'].iloc[i])
        sma20 = float(data['SMA20'].iloc[i])
        sma50 = float(data['SMA50'].iloc[i])
        
        # Skip rows where SMAs are not yet calculated
        if pd.isna(sma50):
            history.append(balance)
            continue

        # Strategy Logic: Golden Cross (Buy) / Death Cross (Sell)
        if sma20 > sma50 and state == "IDLE":
            # BUY: Use all USD balance to buy BTC
            btc_held = balance / price
            balance = 0
            state = "HOLDING"
            print(f"[{data.index[i].date()}] BUY at ${price:.2f}")

        elif sma20 < sma50 and state == "HOLDING":
            # SELL: Convert all BTC back to USD
            balance = btc_held * price
            btc_held = 0
            state = "IDLE"
            print(f"[{data.index[i].date()}] SELL at ${price:.2f} | Current Balance: ${balance:.2f}")

        # Track total value (Cash + Value of BTC)
        current_value = balance + (btc_held * price)
        history.append(current_value)

    # 5. Final Results
    final_value = balance + (btc_held * float(data['Close'].iloc[-1]))
    profit = final_value - initial_balance
    roi = (profit / initial_balance) * 100

    print("\n--- Simulation Results ---")
    print(f"Initial Investment: ${initial_balance}")
    print(f"Final Portfolio Value: ${final_value:.2f}")
    print(f"Total Profit/Loss: ${profit:.2f}")
    print(f"Return on Investment: {roi:.2f}%")

    # 6. Visualization
    data['PortfolioValue'] = history
    plt.figure(figsize=(12, 6))
    plt.plot(data['Close'], label='BTC Price (USD)', alpha=0.5)
    plt.plot(data['SMA20'], label='20-day SMA', color='orange')
    plt.plot(data['SMA50'], label='50-day SMA', color='red')
    plt.title(f"Bitcoin Trading Simulation ({symbol})")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    run_simulation()
