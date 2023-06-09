if __name__ == '__main__':
    header = None
    data = {}

    with open("data.csv", "r") as r:
        for line in r:
            if header is None:
                header = line
            else:
                line_splitted = line.strip().split(",")
                data[int(line_splitted[0])] = (line_splitted[1], line_splitted[2])

    with open("../../doc/data/min_hint.csv", "w") as w:
        w.write(header)

        for common_links in sorted(data.keys()):
            if common_links <= 40:
                w.write(f"{common_links},{data[common_links][0]},{data[common_links][1]}\n")
