from multiprocessing import Pool
import signal
import sys

def handler(a, b):
    print("stop")
    sys.exit(0)

def handler2(a,b):
    print("basta")
    sys.exit(0)

def work(elelemt):
    signal.signal(signal.SIGINT, handler)
    while True:
        pass

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler2)
    pool = Pool()

    array = []

    for i in range(20):
        array.append(i)

    pool.map(work, array)

    while True:
        pool.terminate()
        pool.close()
        pass
