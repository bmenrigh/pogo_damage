#!/usr/bin/env python3

from z3 import *

# This program proves a transitivity property of CMP checks
# for Pikachu powered up to level 11.  That is, if Pikachu A and
# Pikachu B both tie CMP against Pikachu C, Then the CPM of A and B
# must be equal
#
# The s32 model makes the assumption that all the math is done in
# single precision and that the CPM check also only has single
# precision.
#
# The s64 model makes the assumption that CPM values and constants
# start out as 32 bit single precision floats but that the math and
# CMP check is done to double precision.

rm = RNE() # Rounding Nearest Ties to Even

s32 = Solver()
s64 = Solver()

cpm_a_32 = FP("cpm_a_32", Float32())
cpm_b_32 = FP("cpm_b_32", Float32())
cpm_c_32 = FP("cpm_c_32", Float32())

# For a generic reasonable CPM range
#s.add(0.09 <= cpm_a, cpm_a <= 0.9)
#s.add(0.09 <= cpm_b, cpm_b <= 0.9)
#s.add(0.09 <= cpm_c, cpm_c <= 0.9)

# For a CPM approximately at level 11: 0.44310755f
#cpm_l = FPVal("0.4431074559688568115234375", Float32()) # Low -3 floats
#cpm_h = FPVal("0.4431076347827911376953125", Float32()) # High +3 floats

#Float level 11.5 low: 0.453059852123260498046875000000 (0x3ee7f776)
#Float level 11.5 high: 0.453060030937194824218750000000 (0x3ee7f77c)
cpm_l = FPVal("0.453059852123260498046875", Float32()) # Low -3 floats
cpm_h = FPVal("0.45306003093719482421875", Float32()) # High +3 floats

cpm_l_32 = FP("cpm_l_32", Float32())
cpm_h_32 = FP("cpm_h_32", Float32())

s32.add(cpm_l_32 == cpm_l)
s32.add(cpm_h_32 == cpm_h)

s32.add(cpm_l_32 <= cpm_a_32, cpm_a_32 <= cpm_h_32)
s32.add(cpm_l_32 <= cpm_b_32, cpm_b_32 <= cpm_h_32)
s32.add(cpm_l_32 <= cpm_c_32, cpm_c_32 <= cpm_h_32)

# We believe the actual CMP value computed is the sum of the caught
# CPM (32 bits) plus and adjustment (also 32 bits).  However, the
# addition is done in double precision which result in a 64 bit
# value. If the caught CPM is really low, like level 1, then the
# density of floats is as lot closer together. There can be as much as
# 4 bits of precision difference so the sum of these two can yield 4
# more significant bits. A single precision float has 24 mantissa bits
# (23 due to normalization which Z3 doesn't do) so we need a 28 bit
# float to hold this sum.

sum_mant = 24 + 4

cpm_a_sum = FP("cpm_a_sum", FPSort(11, sum_mant))
cpm_b_sum = FP("cpm_b_sum", FPSort(11, sum_mant))
cpm_c_sum = FP("cpm_c_sum", FPSort(11, sum_mant))

cpm_a_64 = fpToFP(RNE(), cpm_a_sum, Float64())
cpm_b_64 = fpToFP(RNE(), cpm_b_sum, Float64())
cpm_c_64 = fpToFP(RNE(), cpm_c_sum, Float64())

s64.add(cpm_l_32 == cpm_l)
s64.add(cpm_h_32 == cpm_h)

cpm_l_sum = fpToFP(RNE(), cpm_l_32, FPSort(11, sum_mant))
cpm_h_sum = fpToFP(RNE(), cpm_h_32, FPSort(11, sum_mant))

s64.add(cpm_l_sum <= cpm_a_sum, cpm_a_sum <= cpm_h_sum)
s64.add(cpm_l_sum <= cpm_b_sum, cpm_b_sum <= cpm_h_sum)
s64.add(cpm_l_sum <= cpm_c_sum, cpm_c_sum <= cpm_h_sum)


# Attack for pikachu with 15 atk_iv
atk_literal = FPVal(112 + 15, Float32())

atk_32 = FP("atk_32", Float32())
s32.add(atk_32 == atk_literal)

atk_64 = fpToFP(RNE(), atk_32, Float64())
s64.add(atk_32 == atk_literal)


atk_a_32 = fpMul(rm, cpm_a_32, atk_32)
atk_b_32 = fpMul(rm, cpm_b_32, atk_32)
atk_c_32 = fpMul(rm, cpm_c_32, atk_32)

atk_a_64 = fpMul(rm, cpm_a_64, atk_64)
atk_b_64 = fpMul(rm, cpm_b_64, atk_64)
atk_c_64 = fpMul(rm, cpm_c_64, atk_64)


# We get CMP ties as floats
#s32.add(atk_a_32 == atk_c_32, atk_b_32 == atk_c_32)

# See if cpm_a can be different than cpm_b
#s32.add(cpm_a_32 != cpm_b_32)

# a > c, a == b
s32.add(atk_a_32 > atk_c_32, atk_a_32 == atk_b_32)

# See if cpm_b can be less than or equal to cpm_c
s32.add(cpm_b_32 <= cpm_c_32)


# We get CMP ties as doubles
#s64.add(atk_a_64 == atk_c_64, atk_b_64 == atk_c_64)

# See if cpm_a can be different than cpm_b
#s64.add(cpm_a_64 != cpm_b_64)

# a > c, a == b
s64.add(atk_a_64 > atk_c_64, atk_a_64 == atk_b_64)

# See if cpm_b can be less than or equal to cpm_c
s64.add(cpm_b_64 <= cpm_c_64)

res32 = s32.check()
print(res32)
if res32 == sat:
    m32 = s32.model()
    print(m32.eval(atk_32))
    print(m32.eval(cpm_a_32))
    print(m32.eval(cpm_b_32))
    print(m32.eval(cpm_c_32))


res64 = s64.check()
print(res64)
if res64 == sat:
    m64 = s64.model()
    print(m64.eval(atk_64))
    print(m64.eval(cpm_a_64))
    print(m64.eval(cpm_b_64))
    print(m64.eval(cpm_c_64))
