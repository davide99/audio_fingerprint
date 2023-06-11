if __name__ == '__main__':
    with open("../../doc/data/recording_length_success_rate.csv", "w") as w:
        w.write("snr,length,percentage_true,percentage_false\n")

        with open("data.csv", "r") as r:
            for line in r:
                w.write(r.read())
