from requests import get

USE_CACHE = True

def indices(start: str, stream: str):
    return [i for i in range(len(stream)) if stream[i:i+len()]]

if __name__ == '__main__':
    if USE_CACHE:
        txt = None
        with open('cache.txt', 'r') as f:
            txt = f.read()
        assert txt is not None
        txt = ''.join(list(set(txt)))
        print(txt)
    else:
        txt = []
        for i in range(0, 200):
            txt.append(get(f'https://0ijq1i6sp1.execute-api.us-east-1.amazonaws.com/dev/stream?q={i}').text)
            # print(txt) # XXX
        txt = map(str.strip, txt)
        txt = map(str.lower, txt)
        txt = map(lambda s: ''.join(list(filter(lambda c: ord('a') <= ord(c) and ord(c) <= ord('z'), s))), txt)
        txt = ''.join(txt)
        print(txt)
        with open('cache.txt', 'w') as f:
            f.write(txt)

# idea was look at the statistics, see that there are few letters, maybe the letter
# are what matter and not like a cyclic thing
# 
# wrought