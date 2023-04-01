# The message was
# '  File "/var/task/app.py", line 258, in get\n    raise InvalidUserAgent(expected_user_agent)\nMozilla/8.8 (Macintosh; Intel Mac OS X 8888_8888) AppleWebKit/888.8.88 (KHTML, like Gecko) Version/88.8.8 Safari/888.8.88'

import requests

url = "https://0ijq1i6sp1.execute-api.us-east-1.amazonaws.com/dev/browser"
if __name__ == "__main__":
    user_agent = "Mozilla/8.8 (Macintosh; Intel Mac OS X 8888_8888) AppleWebKit/888.8.88 (KHTML, like Gecko) Version/88.8.8 Safari/888.8.88"
    print(requests.get(url, headers={"User-Agent": user_agent}).text)

# Response is 
# `blaming`
# get with the expected user agent