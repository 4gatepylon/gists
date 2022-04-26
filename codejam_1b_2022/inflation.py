def least_presses(value, customers):
    return 0

if __name__ == "__main__":
    T = int(input())
    out = ""
    for t in range(T):
        N, P = (int(x) for x in input().split(" "))
        customers = []
        for _ in range(N):
            customer = [int(x) for x in input().split(" ")]
            customers.append(customer)
        out += f"Case #{t+1}: {least_presses(0, customers)}\n"
    out = out[:-1]
    print(out)