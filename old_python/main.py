import sys
import genetic

if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Usage: {0} [input_file] output_file".format(sys.argv[0]))
        sys.exit(0)

    if len(sys.argv) == 2:
        genetic.start(output_file = sys.argv[1])
    else:
        genetic.start(sys.argv[2], sys.argv[1])
