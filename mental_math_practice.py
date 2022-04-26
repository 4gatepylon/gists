import random

from datetime import datetime

LOW = input("Lower bound inclusive")
LOW = int(LOW)
HIGH = input("Upper bound inclusive")
HIGH = int(HIGH)
OPS = ["*"]#["+", "-", "*", "/"]
NUM_TRIES = int(input("Number of tries"))
opmap = {
    "+": lambda x, y: x + y,
    "-": lambda x, y: x - y,
    "*": lambda x, y: x * y,
    "/": lambda x, y: x / y,
}

start_time = datetime.now()
num_correct = 0
for _ in range(NUM_TRIES):
    m1 = int(random.random() * (HIGH - LOW + 1) + LOW)
    m2 = int(random.random() * (HIGH - LOW + 1) + LOW)
    op = random.choice(OPS)
    print(f"{m1} {op} {m2} = ")
    ans = int(input())
    if ans == opmap[op](m1, m2):
        num_correct += 1
end_time = datetime.now()
mins = (end_time - start_time).seconds / 60.0
avg_mins = mins / NUM_TRIES
print(f"Got {num_correct}/{NUM_TRIES} in ~ {mins} mins = {avg_mins} mins/try")