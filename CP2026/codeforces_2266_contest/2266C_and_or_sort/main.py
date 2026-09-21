import sys


def solve(s: str) -> int:
    """Return the minimum operations needed to sort one binary string."""
    # You need to OR everything after the chosen first 1 in the final
    # string. So basically you pick the first 1 location and then OR all
    # 0's after it and AND all 1's before it (assuming you start with at
    # least one 0). If you start with a 1, then you MUST do only the OR
    # operation.
    if len(set(s)) == 1:
        return 0
    if s[0] == '1':
        # All 0's => 1's
        return sum(s[i] == '0' for i in range(1, len(s)))
    # Otherwise, we start with a 0, so we can pick where to 
    # make our final 1

    # NOTE: this quickfix will fix up the "first 1" trick
    # (our algorithm works so long as there will exist at least one 1 that we keep
    # but there exists situations in which that's not the case; all those situations
    # map to one of THESE situations when there is a 1 at the end)
    if s[-1] != '1':
        s += '1'

    # Store running sums
    zeros_before_inclusive = [1 - int(s[0])]
    ones_before_inclusive = [int(s[0])]
    for i in range(1, len(s)):
        if s[i] == '0':
            zeros_before_inclusive.append(zeros_before_inclusive[-1] + 1)
            ones_before_inclusive.append(ones_before_inclusive[-1])
        else:
            ones_before_inclusive.append(ones_before_inclusive[-1] + 1)
            zeros_before_inclusive.append(zeros_before_inclusive[-1])
    
    min_cost = None
    for i in range(len(s)):
        if s[i] == '1':
            n_ones_before_this = ones_before_inclusive[i] - 1
            n_zeros_after_this = zeros_before_inclusive[len(s)-1] - zeros_before_inclusive[i]
            total_cost = n_ones_before_this + n_zeros_after_this
            if min_cost is None or total_cost < min_cost:
                min_cost = total_cost

    assert min_cost is not None, f"String={s}"
    return min_cost


def main() -> None:
    tokens = iter(sys.stdin.buffer.read().split())
    test_cases = int(next(tokens))
    output = []
    for _ in range(test_cases):
        n = int(next(tokens))
        s = next(tokens).decode()
        if len(s) != n:
            raise ValueError("String length does not match n")
        output.append(str(solve(s)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
