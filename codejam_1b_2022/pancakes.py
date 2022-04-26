def max_pay(pancakes):
    i = 0
    j = len(pancakes) - 1
    prev_max = min(pancakes[0], pancakes[-1])
    pays = 0
    while i <= j:
        if pancakes[i] < pancakes[j]:
            if pancakes[i] >= prev_max:
                prev_max = pancakes[i]
                pays += 1
            i += 1
        else:
            if pancakes[j] >= prev_max:
                prev_max = pancakes[j]
                pays += 1
            j -= 1
    return pays

if __name__ == "__main__":
    T = int(input())
    out = ""
    for t in range(T):
        N = int(input())
        pancakes = [int(x) for x in input().split(" ")]
        out += f"Case #{t+1}: {max_pay(pancakes)}\n"
    out = out[:-1]
    print(out)

# Example
# 4
# 2
# 1 5
# 4
# 1 4 2 3
# 5
# 10 10 10 10 10
# 4
# 7 1 3 1000000
