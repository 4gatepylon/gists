readtuple = lambda s: tuple(map(int, map(str.strip, s.strip().split(" "))))

def main():
    s = input()
    d, sumTime = readtuple(s)
    sumOfMins, sumOfMaxes = 0.0, 0.0
    mins = [None] * d
    maxs = [None] * d
    for i in range(d):
        s = input()
        mins[i], maxs[i] = readtuple(s)
        sumOfMins += mins[i]
        sumOfMaxes += maxs[i]
    if sumTime > sumOfMaxes or sumTime < sumOfMins:
        print("NO")
    else:
        amounts = [None] * d
        remaining = sumTime
        for i in range(d):
            assert sumOfMins <= remaining and remaining <= sumOfMaxes
            remainingSumOfMins = sumOfMins - mins[i]
            remainingSumOfMaxs = sumOfMaxes - maxs[i]

            # See how much we'd have left if we took the minimum possible
            remainingRemaining = remaining - mins[i]

            # Look at the clearance (how far above the maximal we should stay under is)
            clearance = remainingSumOfMaxs - remainingRemaining
            minTake = mins[i] if clearance > 0 else mins[i] - clearance
            minTake = int(minTake)
            # print(f"min take is {minTake} with clearance {clearance}, should be between {mins[i]} and {maxs[i]}")
            assert mins[i] <= minTake and minTake <= maxs[i]

            take = min(max(mins[i], minTake), maxs[i])
            amounts[i] = take
            remaining -= take
            
            sumOfMins = remainingSumOfMins
            sumOfMaxes = remainingSumOfMaxs
            assert sumOfMins <= remaining and remaining <= sumOfMaxes

        print(f"YES\n{' '.join(map(str, amounts))}")
    

if __name__ == "__main__":
    main()