import requests
import json

def get_bitcoin_data():
    print("--- 🚀 Bitcoin Lab Experiment ---")
    
    # 1. Fetching Real-Time Price from Public API
    url = "https://api.coindesk.com/v1/bpi/currentprice.json"
    
    try:
        response = requests.get(url)
        data = response.json()
        
        usd_price = data['bpi']['USD']['rate_float']
        updated_time = data['time']['updated']
        
        print(f"Current Price: ${usd_price:,.2f} USD")
        print(f"Last Updated: {updated_time}")
        
        # 2. Conversion Test
        investment = 100  # $100 USD
        btc_value = investment / usd_price
        print(f"\nTest: ${investment} USD would buy {btc_value:.8f} BTC")
        
    except Exception as e:
        print(f"Error fetching data: {e}")

def validate_mock_address(address):
    # Simple logic test: Bitcoin addresses usually start with 1, 3, or bc1
    print(f"\n--- 🔍 Address Validation Test ---")
    if address.startswith(('1', '3', 'bc1')) and 25 <= len(address) <= 62:
        print(f"Address {address[:10]}... is formally VALID.")
    else:
        print(f"Address {address} is INVALID.")

if __name__ == "__main__":
    get_bitcoin_data()
    validate_mock_address("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa") # Satoshi's Address
