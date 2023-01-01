# For every permutation, let us number the elements
# 1, 2, ... , n
# Once we have gotten the n - 1 first elements, we notice that 
# it is necessary that there by only one element not the n-th element
# in the multi-set for Koxia to remove it and force Mahiru to pick
# the n-th element, completing the permutation (the goal is to create
# a permutation).


# We must pick the array C
# Each round they must select form the 3 numbers in a_i, b_i, c_i

# I think order of turns does not matter...
# (for any winning combination you'd just make that move at a different point in time)

# How Mahiru Wins:
# 1. There two numbers a_i and b_i are already outside of the range [n]
#   => Need to pick a number in the range every single round
#
# 2. There is a number in [n] that is not present in a nor in b (because you'd have to add it,
# but then mahiru wouldn't pick it, and you'd have a hole, you need to be able to force him to pick it
# in a single move)
#
# 3. TODO any time you have the set of indices for a value in [n] (i.e. you must pick one of these indices)
#    and for that set the overlap with another such set (in the other side) is greather than 1 and one is not included
#    in the other I think you are screwed, because you can't add a number out of range, you can't add either of those 
#    two numbers to both
#    

# So basically
# 1. Every number in [n] is present in either a or b at one ore more indices
# 2. Every index has at most one number out of bounds
# 3. TODO

# At most 2^n possible permutations to create
# Number of permutations N, if we could count valid constructions at a suffix would be
# N(i) = N(i+1 | a_i) + N(i+1 | b_i)

# Somehow want DP on index perhaps, but DP is hard because it takes O(n) to look up the DP solution
# for your prefix

# You can't make Mahiru color something you add if it wasn't there already
# When would Mahiru take something you added? Only if when you add it it's no good, so why would you add anything
# that does not force Mahiru's hand?
# 1 1
# 2 2

#  At each element
#  - If a number shows up twice we must take it (if two numbers out of range fail or if you had to take 
#    that number before, somehow)
#  - If a number is invalid (out of range or somehow taken before) you need to take the other one
#  - Otherwise, you can take either one or the other


# Is this 2-colorability in an n-partite graph?
# Basically, 