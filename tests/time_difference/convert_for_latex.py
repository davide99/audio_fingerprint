if __name__ == '__main__':
    with open('data.csv', 'r') as r:
        with open("../../doc/data/matching_time_difference.csv", "w") as w:
            w.write("original,recording\n")

            for line in r:
                w.write(line)
