from functools import cmp_to_key
from __future__ import annotations

readtuple2 = lambda: tuple(map(int, map(str.strip, input().strip().split(" "))))
readtuple3 = lambda: tuple(map(int, map(str.strip, input().strip().split(" "))))

Envelope = tuple[int, int, int]

def beats(e1: Envelope, e2: Envelope) -> bool:
    # Whether e1 beats e2
    w1, h1 = e1
    w2, h2 = e2
    return w1 > w2 and h1 > h2

def main():
    n, w, h = readtuple3()

    start: Envelope = (w, h)
    envelopes: set[Envelope] = set([start])
    for _ in range(n):
        w, h = readtuple2()
        envelope: Envelope = (w, h)
        envelopes.add(envelope)
    envelopes: list[Envelope] = list(envelopes)
    envelopes = sorted(envelopes, key=cmp_to_key(beats)) # TODO this won't work...
    raise NotImplementedError # TODO


if __name__ == "__main__":
    main()