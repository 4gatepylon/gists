"""
TODO(hadriano) would be cool to look into tree factorization
TODO(hadriano) look into sieve of eratosthenes (which I forgot lmao)
TODO(hadriano) look into better runtime ANALYSIS (this seems to work but idk why it's fast enough)
TODO(hadriano) look into cross-k sharing somehow for the min cost cache
"""

import sys
from typing import List, Tuple, Iterator, Dict
import logging


# O(max prime factor less than your value) <= O(v) <= O(n)
#
# DECLARE: O(n) runtime
def produce_prime_factors_single(
    v: int,
    prime_factors: dict[int, tuple[int, int]],
    PRIMES_LIST: List[int],
    primes_list_index: int,
) -> List[int]:
    if v in prime_factors:
        return
    for i in range(primes_list_index, len(PRIMES_LIST)):
        p = PRIMES_LIST[i]
        if v % p != 0:
            continue
        # This next step will exit the loop
        # First trivially handle ALL cases of this prime factor (to enable the "one visit per prime" optimization using the primes list index)
        while v % p == 0:
            d = v // p
            prime_factors[v] = d
            v = d
        # Then recurse on remaining primes
        produce_prime_factors_single(d, prime_factors, PRIMES_LIST, i + 1)
        return
    # If you made it here you are prime
    prime_factors[v] = 1


# Produce a DP cache that can be reused across test-cases
# O(n) runetime, each one taking O(n)
#
# DECLARE: O(n**2) runtime
def produce_prime_factors(
    values: int, PRIMES_LIST: List[int], prime_factors: dict[int, int]
) -> None:
    prime_factors[1] = 1
    for v in values:
        # print("> produce_prime_factors_single v:", v, file=sys.stderr) # DEBUG
        produce_prime_factors_single(v, prime_factors, PRIMES_LIST, 0)
    assert all(
        v in prime_factors for v in values
    ), "All prime factors for all values should be computed"


# Read the cache by yielding through the "pointers" until we hit a prime
# O(num prime factors) <= O(log(v)) <= O(log()) since all a_i <= N
#
# DECLARE: O(log(n)) runtime
def get_prime_factors(v: int, prime_factors: dict[int, int]) -> Iterator[int]:
    if v not in prime_factors:
        raise ValueError(f"Value {v} not found in prime factors cache")
    if v == 1:
        raise ValueError(f"Value {v} is 1, which means something is wrong in this call")
    next = prime_factors[v]
    while v != 1:
        # print(f">>>>> next={next}, v={v}", file=sys.stderr) # DEBUG
        # Make sure this is sane
        assert next != v, f"If equal, then we hit 1. Right now, next={next}, v={v}"
        assert v > next, f"v should be greater than next. Right now, v={v}, next={next}"
        assert v % next == 0, f"v should be divisible by next. Right now, v={v}, next={next}"

        # Extract and yield the prime (this could also be cached) with a sanity check
        p = v // next
        # assert is_prime(
        #     p
        # )  # This should usually not be run; we use a shitty sqrt(n) algorithm for the debugging only
        yield p

        # Get the next step
        v, next = next, prime_factors[next]


# Solve with maximal reuse (one)
# Takes O(num prime factors) * sub-solve
#
# In all cases except first sub-solve will take O(num prime factors) <= O(log(n))
# (let's assume for seperate analysis in `solve` that sub-solve happens "seperately)
def solve_single(
    v: int,
    k: int,
    min_operations_to_get_under_k: Dict[int, int],
    prime_factors: dict[int, int],
) -> int:
    # print("CALL(solve_single): v,k =", v,k, file=sys.stderr) # DEBUG
    if v <= k:
        return 0
    if min_operations_to_get_under_k.get(v, None) is not None:
        min_cost = min_operations_to_get_under_k[v]
        assert isinstance(
            min_cost, int
        ), f"Minimum cost should be an integer, but got {type(min_cost)}"
        return min_cost
    these_prime_factors: list[int] = list(get_prime_factors(v, prime_factors))
    min_cost = None
    for p in these_prime_factors:
        d = v // p
        # print(f" >> p, d @ v={v}:", p, d, file=sys.stderr) # DEBUG1
        this_cost_minus_1 = solve_single(
            d, k, min_operations_to_get_under_k, prime_factors
        )
        # print(f" >> this_cost_minus_1 @ v,p ={v},{p}:", this_cost_minus_1, file=sys.stderr) # DEBUG
        assert isinstance(
            this_cost_minus_1, int
        ), f"Solution should be an integer, but got {type(this_cost_minus_1)}"
        this_cost = p * this_cost_minus_1 + 1
        if min_cost is None or this_cost < min_cost:
            min_cost = this_cost
    assert (
        min_cost is not None
    ), f"Minimum cost should be set. Never got it for {v}. Prime factors: {list(prime_factors)}"
    min_operations_to_get_under_k[v] = min_cost
    return min_cost


# Solve with maximal reused (all)
# Takes
# 0. Primes already computed: 0 but worst case O(n * sqrt(n))
# 1. O(n**2) to find the prime factors of all values
# 2. O(n) steps through solve single, each of which takes <= O(log(n))
#    - O(nlog(n))
#    - Each depth/link needs to be calculated once; the depth is at most log(n) and the branching factor is at most log(n)
#      and this need to happen for at most n values (note I say this because each value is at most n), so the maximal cost is
#      O(n * log(n) ** log(n)). log(n) ** log(n) is MORE asymptotically than e ** log(n) = n for constant e. Therefore, this is
#      worse than n. Likewise, it's worse than (e**C)**log(n) = e**(C*log(n)) = n**C for constant C. Therefore, it is worse than
#      any possible polynomial. HOWEVER for our ranges it's probably not that bad... surely... (actually looks kinda bad but idk; my
#      analysis is probably not very accurate; these are all bounds)
#
# DECLARE: O(n * log(n) ** log(n)) runtime (worst case)
def solve(
    values: List[int], k: int, PRIMES_LIST: List[int], PRIME_FACTORS: dict[int, int]
) -> int:
    """Return the minimum operations to make every multiset value at most k."""
    assert isinstance(values, list), f"Values should be a list, but got {type(values)}"
    assert isinstance(k, int), f"k should be an integer, but got {type(k)}"
    assert isinstance(
        PRIMES_LIST, list
    ), f"PRIMES_LIST should be a list, but got {type(PRIMES_LIST)}"
    assert isinstance(
        PRIME_FACTORS, dict
    ), f"PRIME_FACTORS should be a dict, but got {type(PRIME_FACTORS)}"

    # 0. Primes list is already computed
    # 1. Update the prime factors
    # print("@" * 100, file=sys.stderr) # DEBUG
    # print("Producing prime factors", file=sys.stderr) # DEBUG
    produce_prime_factors(values, PRIMES_LIST, PRIME_FACTORS) # KEEP
    # print(f"Values: {values}", file=sys.stderr) # DEBUG
    # print(f"Primes list first 10: {PRIMES_LIST[:10]}", file=sys.stderr) # DEBUG
    # print(f"Prime factors: {PRIME_FACTORS}", file=sys.stderr) # DEBUG
    # print("+" * 100, file=sys.stderr) # DEBUG

    # 2. Find individual costs
    min_costs = []
    min_operations_to_get_under_k: dict[int, int] = {}
    for v in values:
        # print("> solve_single v:", v, file=sys.stderr) # DEBUG
        sol = solve_single(v, k, min_operations_to_get_under_k, PRIME_FACTORS)
        assert isinstance(
            sol, int
        ), f"Solution should be an integer, but got {type(sol)}"
        min_costs.append(sol)

    # Reduce individual cost to total costs (these can be treated independently)
    return sum(min_costs)


# O(sqrt(n)) algorithm for the debugging only
def is_prime(n: int) -> bool:
    if n <= 1:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True


def get_primes_list(n: int) -> List[int]:
    primes = []
    for i in range(2, n + 1):
        if is_prime(i):
            primes.append(i)
    return primes


def main() -> None:
    tokens = iter(map(int, sys.stdin.buffer.read().split()))
    test_cases = next(tokens)
    output = []
    PRIMES_LIST = get_primes_list(2 * 10**5)
    PRIME_FACTORS: dict[int, int] = {}
    for _ in range(test_cases):
        n, k = next(tokens), next(tokens)
        values = [next(tokens) for _ in range(n)]
        output.append(str(solve(values, k, PRIMES_LIST, PRIME_FACTORS)))
    sys.stdout.write("\n".join(output) + "\n")


if __name__ == "__main__":
    main()
