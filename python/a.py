import random
import math

n = 1000
width = 2
sum = 0
for i in range(n):
    x_i = random.uniform(2, 4)
    fx = math.pow(x_i, 3) - 5 * math.pow(x_i, 2) + 5 * x_i + 10
    rec = fx * width
    sum += rec

print('Result: %.5f' % (sum/n))
