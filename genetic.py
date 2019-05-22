import serializer
import signal
import sys
import random
import os
import threading
from solution import Solution
import parameters as par
from multiprocessing import Manager, Pool

#other things
population = []

manager = Manager()
shared_list = manager.list()
file_name_out = ""

#handlers for signal
def handler_main(signum, frame):
    global file_name_out
    serializer.write_to_file(population, file_name_out)
    print("{0} Exiting main process..".format(os.getpid()))
    sys.exit(0)

def handler_worker(signum, frame):
    print("Stopping work..")
    sys.exit(0)

#functions used in multiprocessing
#TODO
def local_search(child):
    global shared_list
    signal.signal(signal.SIGINT, handler_worker)

    child.objf = random.randint(0, 5000)

    shared_list.append(child)

def compute_objf(array):
    global shared_list
    signal.signal(signal.SIGINT, handler_worker)

    objf = -1

    #start_analysys..
    #TODO
    objf = random.randint(0,5000)

    sol = Solution(array, objf)

    shared_list.append(sol)

pool = Pool()

def start(output_file, input_file = ""):
    global file_name_out
    file_name_out = output_file
    signal.signal(signal.SIGINT, handler_main)
    global population
    global pool
    global shared_list

    #setting the starting population
    if len(input_file) != 0:
        population = serializer.read_from_file(input_file)

    #if there are more solutions than POPULATION_SIZE, reduce array
    population = population[0:par.POPULATION_SIZE]
    #if there are less solutions than POPULATION_SIZE, add them
    initialize_solutions()

    print_solutions()

    population.sort(key=lambda solution: solution.objf, reverse=True)


    while True:
        print("Generation started.")

        #generate children
        children = combination()

        pool.map(local_search, children)

        del children[:]
        while len(shared_list) > 0:
            children.append(shared_list.pop(0))

        #new population
        del population[:]
        population = children.copy()
        del children[:]

        population.sort(key=lambda solution: solution.objf, reverse=True)
        print_solutions()

        print("Generation ended.")


def initialize_solutions():
    global population
    global pool
    global shared_list

    numsol = par.POPULATION_SIZE - len(population)

    if numsol == 0:
        return

    new_sols = []

    for i in range(numsol):
        new_sols.append(new_random_solution())

    pool.map(compute_objf, new_sols)

    while len(shared_list) > 0:
        population.append(shared_list.pop(0))


def new_random_solution():
    array = []

    probability = round(par.POPULATION_SIZE / par.AVERAGE_NUM_COLUMNS)

    for i in range(20):
        if random.randint(1, probability) == 1:
            array.append(True)
        else:
            array.append(False)

    return array



def print_solutions():
    global population

    #print("Number of elements: {0}".format(len(population)))

    for solution in population:
        print(solution.array)
        print(solution.objf)

#TODO
def combination():
    global population
    return population.copy()

#TODO
def check_best_solution():
    pass
