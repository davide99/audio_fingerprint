import glob

if __name__ == '__main__':
    values = {}
    maxes = {}

    for file in glob.glob('data_*.csv'):
        window_name = file[5:-4]

        with open(file, "r") as f:
            for line in f:
                splitted_line = line.strip().split(",")
                db = splitted_line[0]
                rate = float(splitted_line[1])

                if db not in values:
                    values[db] = {}

                values[db][window_name] = rate
                if window_name not in maxes:
                    maxes[window_name] = rate
                elif maxes[window_name] < rate:
                    maxes[window_name] = rate

    with open("../../doc/data/windows_success_rate.csv", "w") as f:
        first = True

        for db in values:
            if first:
                f.write("db")
                for window_name, _ in sorted(values[db].items()):
                    f.write("," + window_name)
                f.write("\n")
                first = False

            f.write(db)
            for window_name, _ in sorted(values[db].items()):
                value = float(values[db][window_name])
                value /= maxes[window_name]
                value *= 100
                f.write(f",{value}")
            f.write("\n")
