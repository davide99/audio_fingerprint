# 🎶 Music recognition system in C++17

Library for recognizing songs using short noisy recordings. It is designed to be
fast without sacrificing code readability.

The average time spent analyzing a song of 3 min is 1.9s on a i5-6200U with
release optimizations turned on.

## Table of Contents

> * [Brief explanation](#explanation)
> * [Configuration](#configuration)
> * [Requirements](#requirements)
> * [Convert an audio to WAV](#convertWav)
> * [Build instructions](#build)
> * [Contributing](#contrib)

## Brief explanation

<a name="explanation"/>

La cartella `Core` contiene l'implementazione dell'algorimo che consiste in:

* *Fingerprint*: divide lo spettro in sezioni (in tempo e in frequenza), per ogni sezione individua i picchi utilizzando
  la funzione definita in PeaksFinder
* *Link*: classe per la rappresentazione dei picchi secondo lo schema indirizzo-picco utilizzata per creare l'hash
* *Links*: contiene la funzione per creare i link
* *Peak*: classe per la rappresentazione dei picchi di intensità
* *PeaksFinder*: classe per individuare i picchi

La cartella `Math` contiene alcune classi di supporto:

* *FFTWindow*: classe per rappresentare una finestra dello spettro
* *Integers*: contiene funzioni per fare operazioni su interi (byte swap e conversione in stringa)
* *Spectrogram*: classe per rappresentare lo spettro
* *Vector*: contiene una funzione per multiplicare due vettori tra di loro (utilizzata durante la finestratura)
* *Window*: contiene funzioni per calcolare la finestra di hamming, la frequenza centrale di ogni bin di frequenze dello
  spettro, le bande Mel, ottenere la banda Mel a partire da una frequenza in Hertz

La cartella `IO` contiene la classi per le operazioni di I/O:

* *DB*: classe per interfacciarsi col database, permette di verificare che il database esista, di crearlo, di
  eliminarlo, di inserire i fingerprint di una canzone, di ricercare l'id di una canzone, ottenere il nome di una
  canzone dato il suo id
* *WavReader*: classe per fare il parse di un file WAV

La cartella `Utils` contiene alcune funzioni di utilità utilizzate nel programma:

* *FixedSizePQ*: priority queue a lunghezza fissa, utilizzata per individuare i picchi a intensità maggiori in una
  sezione dello spettro (vedi classe Core/Fingerprint)
* *Utils*: funzioni per verificare che un file esista, per ottenere l'endianess della macchina, verificare che una
  stringa termini in un determinato modo, ottenere i file in una directory, effettuare il trim di una stringa
* *Wrapper*: semplifica le operazioni di inserimento e ricerca dei brani fungendo da wrapper

Si consiglia di partire dalla funzione `insertSong` in Wrapper per comprendere il funzionamento dell'algoritmo.

## Configuration

<a name="configuration"/>

1. In the `Consts.h` file insert the correct hostname, username and password
   (the database will be automatically created if not existent)
1. Put some WAV files in a directory
1. Run the program with the following parameters: `-i <path to wav files>`
1. Wait until the program exits
1. Record a small piece of song previously in the WAV directory
1. Feed the recording to the program with the following parameters:
   `-s <path to recording>`
1. Eventually a result will be showed

You might also need set to maximum allowed packed size in MariaDB / MySQL.
To do so add `max_allowed_packet=64M` to `/etc/my.cnf` or
`C:\Program Files\MariaDB <ver>\data`.

## Requirements

<a name="requirements"/>

* MariaDB / MySQL
* Some WAV files
* ffmpeg (optional)

## Convert an audio to WAV

<a name="convertWav"/>

`ffmpeg -i audiofile.ext -ar 8000 -ac 1 out.wav`

## Build instructions

<a name="build"/>

### Debian

1. `git submodule update --init --recursive`
1. `sudo apt install libmariadbclient-dev`
1. `sudo apt install libfftw3-dev`

### Windows (MSys2)

1. `pacman -S mingw-w64-x86_64-gcc`
1. `pacman -S mingw-w64-x86_64-gdb`
1. `pacman -S mingw-w64-x86_64-make`
1. `pacman -S mingw-w64-x86_64-cmake`
1. `git submodule update --init --recursive`
1. `pacman -S mingw-w64-x86_64-libmariadbclient`
1. `pacman -S mingw-w64-x86_64-fftw`
