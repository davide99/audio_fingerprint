# 🎶 Music recognition system in C++17

Library for recognizing songs using short noisy recordings. It is designed to be
fast without sacrificing code readability.

The average time spent analyzing a song of 3 min is 0.2s on a i5-2500 with
release optimizations turned on.

## Table of Contents

> * [Configuration](#configuration)
> * [Requirements](#requirements)
> * [Convert an audio to WAV](#convertWav)
> * [Build instructions](#build)

## Configuration

<a name="configuration"/>

1. In the `consts.h` file insert the correct HOSTNAME, USERNAME and PASSWORD
   (the database will be automatically created if not existent)
1. You might also need set to maximum allowed packed size in MariaDB / MySQL.
   To do so add `max_allowed_packet=64M` to `/etc/my.cnf` or
   `C:\Program Files\MariaDB <ver>\data`.

## Requirements

<a name="requirements"/>

* MariaDB / MySQL
* ffmpeg (optional)

## Convert an audio to WAV

<a name="convertWav"/>

`ffmpeg -i audiofile.ext -ar 8000 -ac 1 out.wav`

## Build instructions

<a name="build"/>

### Debian

1. `git submodule update --init --recursive`
1. `sudo apt install libmariadb-dev`
1. `sudo apt install libfftw3-dev`

### Windows (MSys2)

1. `pacman -S mingw-w64-x86_64-gcc`
1. `pacman -S mingw-w64-x86_64-gdb`
1. `pacman -S mingw-w64-x86_64-make`
1. `pacman -S mingw-w64-x86_64-cmake`
1. `git submodule update --init --recursive`
1. `pacman -S mingw-w64-x86_64-libmariadbclient`
1. `pacman -S mingw-w64-x86_64-fftw`
