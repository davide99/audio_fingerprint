import matplotlib.pyplot as plt

if __name__ == '__main__':
    with open('data.csv', 'r') as r:
        with open("../../doc/data/matching_time_difference_hinst.csv", "w") as w:
            w.write("delta\n")

            data = []

            for line in r:
                splitted = line.split(',')
                delta = int(splitted[0]) - int(splitted[1])
                data.append(delta)

                w.write(f"{delta}\n")

            plt.hist(data, bins=50)
            plt.show()
