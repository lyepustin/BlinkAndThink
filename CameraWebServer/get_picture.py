import requests
import os

# Replace 'YOUR_BOT_TOKEN' with your actual Telegram bot token
TELEGRAM_BOT_TOKEN = '****'
TELEGRAM_CHAT_ID = '****'

def download_and_send_image(url):
    response = requests.get(url)
    
    if response.status_code == 200:
        # Get the file extension from the URL
        file_extension = url.split('=')[-1]
        
        # Save the image locally
        with open('image.' + file_extension + ".png", 'wb') as f:
            f.write(response.content)
        
        # Prepare the URL for sending the image via Telegram API
        telegram_api_url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendPhoto"
        
        # Set up the parameters for the POST request
        params = {
            'chat_id': TELEGRAM_CHAT_ID,
        }
        
        files = {
            'photo': open('image.' + file_extension + ".png", 'rb')
        }
        
        # Make the POST request to send the image
        telegram_response = requests.post(telegram_api_url, params=params, files=files)
        files['photo'].close()
        
        if telegram_response.status_code == 200:
            print("Image sent successfully!")
        else:
            print(f"Failed to send image via Telegram API. Status code: {telegram_response.status_code}")
        
        os.remove('image.' + file_extension + ".png",)
    else:
        print(f"Failed to download image. Status code: {response.status_code}")

# Replace 'IMAGE_URL' with the actual URL of the image you want to download and send
image_url = 'http://10.0.0.10/capture?_cb=1702850599022'
download_and_send_image(image_url)
