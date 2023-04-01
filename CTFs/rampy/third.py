import requests

def sm(w, one=1):
    return sum([ord(x) - ord('a') + one for x in w])
def ok(w):
    return sm(w) == 42#sm('fortytwo')

if __name__ == '__main__':
    # print('spaghetti')
    with open('wiki-100k.txt', 'r') as f:
        lines = f.readlines()
        cands = list(filter(lambda x: len(x) == 7 and x[0] != '#', map(lambda x: x.strip().lower(), lines)))
        cands = [c for c in cands if all([ord('a') <= ord(x) and ord(x) <= ord('z') for x in c])]
        cands = [c for c in cands if c[0] != 'h']
        cands = [c for c in cands if ok(c)]
        cands = [c for c in cands if not 'l' in c]
        print(cands)
        # print(sm('elegant'))
        print("********")
        for c in cands:
            print(requests.get(f"https://0ijq1i6sp1.execute-api.us-east-1.amazonaws.com/dev/exception?q={c}").text)

# this: chariot
# (tldr: look at query then the exceptions tell you what to look for)