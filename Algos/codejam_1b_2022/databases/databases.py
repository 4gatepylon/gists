def initial_V():
    return "00110011"

# Not sure how to do this:
# Know that
# 1.
#   If you send in V with k bits
#   You know that k = Adds + Losses
#   You know that D = Change - Previous = Adds - Losses
#   So you know  that (k - D) / 2 = Adds, and Losses = k - Adds
# 2.
#   Obviously, 0 <= n1(A xor B) <= n1(A) + n1(B)
# 3.
#   256 Original Possible X's
#   8^n original possible trajectories given an X
# 4.
#   Given that we know we have N bits, there are 8CN possible X's (i.e. 1, 4, 28, 56, 70, 56, 28, 4, 1)
# 5.
#   Given one move we can usually narrow down what bits might be the ones flipped, but we probably need
#   historical information to further narrow down because you can move back up.
# 6.
#   It's hard making some sort of fixed point algorithm for this, perhaps I have to try to do some
#   sort of intersection of possible states? The problem is that the state changes every time I get
#   information which is what makes it hard.
# 7.
#   It will help to solve smaller cases like 2, 3, 4, 5... (not sure if odd numbers or powers that are
#   not powers of 2 will behave differently). Note that 2 is super obvious. In the case of 3 there seems
#   to be a cycle where the database can spin you forever (i.e. if you have 0 or 3 1's you win, so it
#   would only be interesting with 1 or 2 1's: if there is 1 then if you probe with 3 you get 2, if you
#   probe with 2 it will send you to 2 where anything is possible, and if you probe with 1 it will also
#   send you to 2; if there are 2 then it would send you to 1 if you probed with anything greater than
#   zero).
#  8.
#   Probing with 0 never helps.
# ...
def next_V(N, prev_Vs, prev_n1s, possible_X):
    if N == 8:
        # If N == 8 we can zero it out by using -1
        return "11111111"
    elif N < 4:
        # If N is small, we can get close to 8 1's by using -1
        return "11111111"
    elif N >= 4:
        pass
    else:
        raise Exception(f"Invalid N: {N}")
    return "00000000"

# python3 interactive_runner.py python3 tester.py -- python3 databases.py
if __name__ == "__main__":
    T = int(input())
    for _ in range(T):
        V = initial_V()
        prev_Vs = [V]
        count = 301

        prev_n1s = [None]
        print(V, flush=True)
        N = int(input())
        while N > 0 and count > 0:
            V = next_V(N, prev_Vs)
            prev_Vs.append(V)
            print(V, flush=True)

            N = int(input())
            prev_n1s.append(N)
            count -= 1
            if count == 0 or N == -1:
                raise Exception("Timeout")
            pass
        pass
    pass