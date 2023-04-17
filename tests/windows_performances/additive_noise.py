import os
import glob
from scipy.io import wavfile
import scipy
import scipy.signal
import numpy as np
from subprocess import Popen, PIPE
from collections import defaultdict
import matplotlib.pyplot as plt

input_dir = "/home/davide/Scrivania/wav/"


def add_colored_noise(signal, target_snr):
    # Calcoliamo la potenza del segnale
    signal_power = np.sum(signal ** 2) / len(signal)

    # Calcoliamo la potenza del rumore necessario per raggiungere il target SNR
    noise_power = signal_power / (10 ** (target_snr / 10))

    # Generiamo rumore non bianco con la stessa lunghezza del segnale
    noise = np.random.randn(len(signal))
    # Filtriamo il rumore con un filtro passa-basso per generare rumore non bianco
    b, a = scipy.signal.butter(3, 0.2)
    noise = scipy.signal.filtfilt(b, a, noise)
    # Applichiamo una funzione di soglia per generare rumore ancora più aggressivo
    noise = np.sign(noise) * np.power(np.abs(noise), 1.2)
    # Normalizziamo il rumore in modo che abbia la stessa potenza del rumore bianco
    noise = np.sqrt(noise_power) * noise / np.std(noise)

    # Aggiungiamo il rumore al segnale originale
    noisy_signal = signal + noise

    return noisy_signal


if __name__ == '__main__':
    snrs_list = list(range(-40, 40, 1))  # db
    snr_d = defaultdict(int)

    songs = glob.glob(os.path.join(input_dir, "*.wav"))
    i = 0
    for song in songs:
        sample_rate, signal = wavfile.read(song)
        signal = signal / 32768.0
        name = os.path.basename(song)

        for snr_value in snrs_list:
            windowed_signal = signal[int(len(signal) / 2): int(len(signal) / 2 + sample_rate * 10)]

            noisy_signal = add_colored_noise(windowed_signal, snr_value)
            noisy_signal = noisy_signal / np.max(np.abs(noisy_signal))

            wavfile.write("out.wav", sample_rate, (noisy_signal * 32768.0).astype(np.int16))

            process = Popen(["../../cmake-build-release/mock_client/mock_client", "out.wav"], stdout=PIPE)
            (output, err) = process.communicate()
            exit_code = process.wait()
            output = output.decode('utf-8').strip()

            if output == name:
                snr_d[snr_value] += 1

        i += 1
        print(f">>{i * 100 / len(songs)}%")

    success_rate = [snr_d[snr_value] * 100.0 / len(songs) for snr_value in snrs_list]

    with open('data.csv', 'w') as f:
        for i in range(len(success_rate)):
            f.write(f"{snrs_list[i]},{success_rate[i]}\n")

    plt.plot(snrs_list, success_rate)
    plt.show()
