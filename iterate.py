import os
import time

os.system("make clean")

for i in range(1, 100, 1):
    os.system("clear; make; ./intro " + str(i))
    time.sleep(0.05)

for i in range(100, 800, 5):
    os.system("clear; make; ./intro " + str(i))
    time.sleep(0.05)

for i in range(800, 2000, 100):
    os.system("clear; make; ./intro " + str(i))
    time.sleep(0.05)
