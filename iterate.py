import os
import time

os.system("make clean")

programStr = "./bin/main.out"
clear = False
runStr = f"{'clear;' if clear else ''} make; {programStr} "

for i in range(1, 100, 1):
    os.system(runStr + str(i))
    time.sleep(0.05)

for i in range(100, 800, 5):
    os.system(runStr + str(i))
    time.sleep(0.05)

for i in range(800, 2000, 100):
    os.system(runStr + str(i))
    time.sleep(0.05)
