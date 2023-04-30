if __name__ == '__main__':
    with open("../../doc/data/recording_length_success_rate.csv", "w") as w:
        w.write("length,percentage\n")
        w.write("0,0.0\n")

        with open("data.csv", "r") as r:
            for line in r:
                w.write(r.read())
