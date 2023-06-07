import os
import glob
from scipy.io import wavfile
import numpy as np
from subprocess import Popen, PIPE
from collections import defaultdict
import matplotlib.pyplot as plt

input_dir = "/home/davide/Scrivania/wav/"

if __name__ == '__main__':
    lengths_list = [.5]
    while lengths_list[-1] < 6.0:
        lengths_list.append(lengths_list[-1] + 0.5)

    lengths_d = defaultdict(int)
    false_d = defaultdict(int)

    songs = glob.glob(os.path.join(input_dir, "*.wav"))
    i = 0
    for song in songs:
        sample_rate, signal = wavfile.read(song)
        signal = signal / 32768.0
        name = os.path.basename(song)

        for length in lengths_list:
            num_samples = int(sample_rate * length)
            windowed_signal = signal[0:num_samples]

            wavfile.write("out.wav", sample_rate, (windowed_signal * 32768.0).astype(np.int16))

            process = Popen(["../../cmake-build-release/mock_client/mock_client", "out.wav"], stdout=PIPE)
            (output, err) = process.communicate()
            exit_code = process.wait()
            output = output.decode('utf-8').strip()

            if exit_code == 0:
                # ha trovato qualcosa
                if output == name:
                    lengths_d[length] += 1
                    print("Brano corretto")
                else:
                    false_d[length] += 1
                    print("Falso positivo")

        i += 1
        print(f">>{i * 100 / len(songs)}%")

    success_rate = [lengths_d[length] * 100.0 / len(songs) for length in lengths_list]
    false_positive_rate = [false_d[length] * 100.0 / len(songs) for length in lengths_list]

    with open('data.csv', 'w') as f:
        for i in range(len(success_rate)):
            f.write(f"{lengths_list[i]},{success_rate[i]},{false_positive_rate[i]}\n")

    plt.plot(lengths_list, success_rate)
    plt.plot(lengths_list, false_positive_rate)
    plt.show()
