import os
import glob
from scipy.io import wavfile
from subprocess import Popen, PIPE
from collections import defaultdict
import json

input_dir = "/home/davide/Scrivania/wav/"

if __name__ == '__main__':
    lengths_list = [.5]
    while lengths_list[-1] < 6.0:
        lengths_list.append(lengths_list[-1] + 0.5)

    data = defaultdict(lambda: {
        'true_positive': 0.0,
        'false_positive': 0.0
    })

    songs = glob.glob(os.path.join(input_dir, "*.wav"))
    i = 0
    for song in songs:
        sample_rate, signal = wavfile.read(song)
        name = os.path.basename(song)

        for length in lengths_list:
            num_samples = int(sample_rate * length)
            trimmed_signal = signal[0:num_samples]
            wavfile.write("out.wav", sample_rate, trimmed_signal)

            process = Popen(["../../cmake-build-release/mock_client/mock_client", "out.wav", "--to-json"], stdout=PIPE)
            (output, err) = process.communicate()
            exit_code = process.wait()
            output = output.decode('utf-8').strip()
            output_json = json.loads(output)

            if output_json['found']:
                found_name = output_json['name']
                common_links = output_json['commonLinks']

                if found_name == name:
                    data[common_links]['true_positive'] += 1
                else:
                    data[common_links]['false_positive'] += 1

        i += 1
        print(f">>{i * 100 / len(songs)}%")

    for common_links in data.keys():
        total = data[common_links]['true_positive'] + data[common_links]['false_positive']
        data[common_links]['true_positive'] = data[common_links]['true_positive'] * 100.0 / total
        data[common_links]['false_positive'] = data[common_links]['false_positive'] * 100.0 / total

    with open('data.csv', 'w') as f:
        f.write("common_links,true_positive,false_positive\n")
        for common_links in data.keys():
            f.write(f"{common_links},{data[common_links]['true_positive']},{data[common_links]['false_positive']}\n")
