T = int(input().strip())
ans = []
for _ in range(T):
    t = int(input().strip())
    ans.append("Alice" if t == 1 or t >= 5 else "Bob")
print("\n".join(ans))