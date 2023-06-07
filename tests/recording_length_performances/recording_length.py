import os
import glob
from scipy.io import wavfile
import numpy as np
from subprocess import Popen, PIPE
from collections import defaultdict
import scipy.signal

input_dir = "/home/davide/Scrivania/wav/"


def add_colored_noise(x, target_snr):
    # Calcoliamo la potenza del segnale
    signal_power = np.sum(x ** 2) / len(x)

    # Calcoliamo la potenza del rumore necessario per raggiungere il target SNR
    noise_power = signal_power / (10 ** (target_snr / 10))

    # Generiamo rumore non bianco con la stessa lunghezza del segnale
    noise = np.random.randn(len(x))
    # Filtriamo il rumore con un filtro passa-basso per generare rumore non bianco
    b, a = scipy.signal.butter(3, 0.2)
    noise = scipy.signal.filtfilt(b, a, noise)
    # Applichiamo una funzione di soglia per generare rumore ancora più aggressivo
    noise = np.sign(noise) * np.power(np.abs(noise), 1.2)
    # Normalizziamo il rumore in modo che abbia la stessa potenza del rumore bianco
    noise = np.sqrt(noise_power) * noise / np.std(noise)

    # Aggiungiamo il rumore al segnale originale
    return x + noise


if __name__ == '__main__':
    lengths_list = [.5]
    while lengths_list[-1] < 6.0:
        lengths_list.append(lengths_list[-1] + 0.5)
    snrs_list = [40]  # db

    data = dict()
    for snr_value in snrs_list:
        data[snr_value] = defaultdict(lambda: {
            'true_positive': 0.0,
            'false_positive': 0.0
        })

    songs = glob.glob(os.path.join(input_dir, "*.wav"))
    i = 0
    for song in songs:
        sample_rate, signal = wavfile.read(song)
        signal = signal / 32768.0
        name = os.path.basename(song)

        for snr_value in snrs_list:
            max_num_samples = int(sample_rate * lengths_list[-1])
            max_windowed_signal = signal[0:max_num_samples]
            max_noisy_signal = add_colored_noise(max_windowed_signal, snr_value)
            max_noisy_signal = max_noisy_signal / np.max(np.abs(max_noisy_signal))
            max_noisy_signal = (max_noisy_signal * 32768.0).astype(np.int16)

            for length in lengths_list:
                num_samples = int(sample_rate * length)
                noisy_signal = max_noisy_signal[0:num_samples]

                wavfile.write("out.wav", sample_rate, noisy_signal)

                process = Popen(["../../cmake-build-release/mock_client/mock_client", "out.wav"], stdout=PIPE)
                (output, err) = process.communicate()
                exit_code = process.wait()
                output = output.decode('utf-8').strip()

                if exit_code == 0:
                    # ha trovato qualcosa
                    if output == name:
                        data[snr_value][length]['true_positive'] += 1
                    else:
                        data[snr_value][length]['false_positive'] += 1

        i += 1
        print(f">>{i * 100 / len(songs)}%")

    for snr_value in snrs_list:
        for length in lengths_list:
            data[snr_value][length]['true_positive'] = data[snr_value][length]['true_positive'] * 100.0 / len(songs)
            data[snr_value][length]['false_positive'] = data[snr_value][length]['false_positive'] * 100.0 / len(songs)

    with open('data.csv', 'w') as f:
        for snr_value in snrs_list:
            for length in lengths_list:
                f.write(
                    f"{snr_value},{length},{data[snr_value][length]['true_positive']},{data[snr_value][length]['false_positive']}\n")
