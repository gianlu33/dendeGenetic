import csv
from solution import Solution

def read_from_file(input_file):
    population = []

    with open("data/{0}".format(input_file)) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0

        #read a solution
        for row in csv_reader:

            sol = []
            value = -1
            for i in range(20):
                sol.append(row[i] in ["True"])
            value = int(row[20])

            s = Solution(sol, value)
            population.append(s)

    #for now, the best is the first one
    #TODO, vedere la cosa della best solution
    best_solution = population[0]

    return population


def write_to_file(population, output_file):
    with open("data/{0}".format(output_file), mode='w') as csv_file:
        csv_writer = csv.writer(csv_file)

        for solution in population:
            row = solution.array.copy()
            row.append(solution.objf)

            csv_writer.writerow(row)
