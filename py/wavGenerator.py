import numpy as np
from scipy.io.wavfile import write


# Generate a wave given a file with a sample for each line


def main():
    samples = []

    with open("../out.txt") as f:
        for line in f:
            samples.append(int(line))

    write("../test_out.wav", 8000, np.array(samples, dtype=np.int16))


if __name__ == '__main__':
    main()
