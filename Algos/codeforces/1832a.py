T = int(input().strip())
ans = []
for _ in range(T):
    s = input().strip()
    counts = {}
    for l in s:
        if not l in counts:
            counts[l] = 0
        counts[l] += 1
    ans.append(len(counts) > 2 or len(counts) == 2 and min(counts.values()) > 1)
print("\n".join(("YES" if a else "NO" for a in ans)))