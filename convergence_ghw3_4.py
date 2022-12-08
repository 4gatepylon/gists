import math
assert math.comb(3, 1) == 3 and math.comb(4, 2) == 6

def pp1(n: int, k: int):
    # radius tells you how far out to be as a ratio of n
    # This seems to work for any number > 2 strictly
    r = n/2.11
    if k < n/2 - r or n/2 + r < k:
        return math.comb(n, k)
    else:
        return 2**n

def pp2(n: int, k: int):
    # radius tells you how far out to be as a ratio of n
    # This seems to work for any number > 2 strictly
    r = n/2.11
    if k < n/2 - r or n/2 + r < k:
        return n**min(n - k, k)
    return 2**n

def dd2(n: int, k: int):
    r = n/2.11
    if k < n/2 - r or n/2 + r < k:
        return 2 ** (-2*(n-1) / 2)
    return 2 ** (-k * (n - k)/2)

def sum(n : int):
    s = 0
    for k in range(1, n):
        assert 0 < k
        assert k < n
        # print((n / (n - k))**(n - k))
        # s += math.comb(n, k) * math.sqrt(n) * (n / (n - k))**(n - k) * 2**(-k*(n - k) /2)
        # s += math.comb(n, k)** 2 * 2**(-k*(n - k) /2)
        # s += math.comb(n, k)**2 * 2**(-n**(1.41)/8)
        # s += pp1(n, k)**2 * 2 **(-k*(n-k)/2)
        s += pp2(n, k)**2 * dd2(n, k)
    return s

if __name__ == '__main__':
    for n in range(1, 1000):
        s = sum(n)
        l_bound = 0
        u_bound = (1 + 1/12)**(-2 * n)
        print(l_bound, "<", s, "<", u_bound)