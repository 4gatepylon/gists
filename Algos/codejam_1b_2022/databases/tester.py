def init_X():
    return "10000000"

def next_r(V, X):
    return 1

def rot(V, r):
    if r > len(V):
        raise Exception(f"Invalid r: {r}")
    return V[-r:] + V[:-r]

def xor(V, W):
    if len(V) != len(W):
        raise Exception(f"Invalid V and W: {V} and {W}")
    return "".join(str(int(V[i]) ^ int(W[i])) for i in range(len(V)))

def num1(X):
    return X.count("1")

# Tester to do a single test case based mostly on what the problem statement gave us
if __name__ == "__main__":
    num_tests = 1
    print(num_tests, flush=True)
    
    for _ in range(num_tests):
        X = init_X()
        X_seq = [X]
        count = 301
        n1 = 1
        while count > 0 and n1 > 0:
            V = input()
            r = next_r(V, X)
            W = rot(V, r)
            X = xor(X, W)
            n1 = num1(X)
            print(n1, flush=True)
            count -= 1
        if n1 != 0:
            print("-1", flush=True)
            printable_X_seq = "\n".join(map(lambda s: f"\t{s}", X_seq))
            raise Exception(f"Timeout (tester) with sequence of X's: {printable_X_seq}")
        pass
    pass