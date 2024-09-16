#!/usr/bin/python

import math

def min_dmg(hp, count):
    half_hp = math.floor(hp / 2)
    min_hp = half_hp + 1

    return math.ceil(min_hp / count)


def max_dmg(hp, count):
    half_hp = math.floor(hp / 2)
    max_hp = half_hp

    return math.floor(max_hp / (count - 1))


def possible_count(hp, count):
    return min_dmg(hp, count) <= max_dmg(hp, count)

# Wooloo
#counts = range(23, 33)
#impossible_counts = []

# Squirtle
#counts = [item for inner in [list(range(28, 34)), [35, 36, 37, 39, 48, 54]] for item in inner]
#impossible_counts = [34, 38];

# All known valid counts to yellow
counts = [item for inner in [[8, 9, 11, 12, 14, 15, 17], list(range(23, 34)), [35, 36, 37, 39, 43, 48, 54, 66, 71, 78, 86]] for item in inner]
impossible_counts = [34, 38, 67, 68, 69, 70, 79, 80, 81, 82, 83, 84, 85];



for hp in range(50, 10000):
    possible = True
    for count in counts:
        if possible_count(hp, count) == False:
            possible = False
            break

    for count in impossible_counts:
        if possible_count(hp, count) == True:
            possible = False
            break

    if possible:
        print("Possible HP: %4d" % (hp))
