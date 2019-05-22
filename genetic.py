import serializer
import sys
import random
import threading
from solution import Solution
from multiprocessing.dummy import Pool as ThreadPool
import parameters as par

#other things
population = []
best_solution = Solution()
lock = threading.Lock()
stop = False

def start(output_file, input_file = ""):
    threads = []
    global stop
    global population

    #setting the starting population
    if len(input_file) != 0:
        population = serializer.read_from_file(input_file)

    #if there are more solutions than POPULATION_SIZE, reduce array
    population = population[0:par.POPULATION_SIZE]
    #if there are less solutions than POPULATION_SIZE, add them
    initialize_solutions()

    try:
        while True:
            print("Generation started.")

            #generate children
            children = combination()

            #local search in multithreading
            for i in range(par.POPULATION_SIZE):
                thread = threading.Thread(target=local_search, args=(children[i],))
                threads.append(thread)
                thread.start()

            for thread in threads:
                thread.join()
            del threads[:]

            #new population
            del population[:]
            population = children.copy()
            del children[:]

            print("Generation ended.")


    except KeyboardInterrupt:
        stop = True
        serializer.write_to_file(population, output_file)
        sys.exit(0)


def initialize_solutions():
    global population
    threads = []

    numsol = par.POPULATION_SIZE - len(population)

    for i in range(numsol):
        thread = threading.Thread(target=new_random_solution)
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()
    del threads[:]

def new_random_solution():
    global lock
    global population
    array = []
    value = -1

    probability = round(par.POPULATION_SIZE / par.AVERAGE_NUM_COLUMNS)

    for i in range(20):
        if random.randint(1, probability) == 1:
            array.append(True)
        else:
            array.append(False)

    #start simulation in order to obtain the value..
    #TODO
    value = random.randint(0,5000)

    sol = Solution(array, value)

    with lock:
        population.append(sol)


def print_solutions():
    global population

    print("Number of elements: {0}".format(len(population)))

    for solution in population:
        print(solution.array)
        print(solution.objf)

#TODO
def combination():
    global population
    return population.copy()

#TODO
def local_search(child):
    global stop
    global lock

    while not stop:
        print(threading.currentThread().getName())

#TODO
def check_best_solution():
    global lock
