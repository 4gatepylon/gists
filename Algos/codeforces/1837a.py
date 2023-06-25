readtuple = lambda: tuple(map(int, map(str.strip, input().strip().split(" "))))
T = int(input().strip())
ans = []
for _ in range(T):
    x, k = readtuple()
    ans.append(2 if x % k == 0 else 1)
    sol = x
    if ans[-1] != 1:
        sol = f"{x - 1} 1"
    ans.append(sol)
print("\n".join(map(str, ans)))