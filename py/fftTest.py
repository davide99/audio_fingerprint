import numpy as np
from scipy.io.wavfile import read


# For test purposes


def main():
    fft_ = []

    with open("../fft.txt") as f:
        for line in f:
            fft_.append(float(line))

    _, data = read("../out.wav")

    data = data[:512]
    hamming = np.hanning(512)

    fft = np.absolute(np.fft.rfft(np.multiply(data, hamming)))

    for i in range(len(fft_)):
        print(fft_[i], fft[i])


if __name__ == '__main__':
    main()
