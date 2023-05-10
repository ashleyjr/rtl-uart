import argparse
import matplotlib.pyplot as plt

def main(csv_file, png_file):

    data = {}

    # Read data
    with open(csv_file, 'r') as f:
        csvs = f.read()

    # Change to ints
    for csv in csvs.split("\n"):
        if len(csv) != 0:
            d = csv.split(",")
            print(d)
            assert len(d) == 3

            if not d[1] in data:
                data[d[1]] = [[],[]]
            data[d[1]][0].append(int(d[0]))
            data[d[1]][1].append(int(d[2]))
    # for
    for l in data:
        plt.scatter(data[l][0], data[l][1], label=l)
    plt.legend()
    plt.xscale('log')
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("# Transistors")
    plt.grid()
    plt.savefig(png_file)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--csv_file', type=str, required=True)
    parser.add_argument('--png_file', type=str, required=True)
    args = parser.parse_args()
    main(args.csv_file, args.png_file)
