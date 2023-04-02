#!/usr/bin/env bash

#Inizializzo SDK emscripten
source "$HOME"/emsdk/emsdk_env.sh

#Creo, se non esiste già, cartella di build
BUILD_DIR="linux_build"
mkdir -p $BUILD_DIR
cd $BUILD_DIR || exit

#Build
if [[ $1 == update ]]; then
  echo Rieseguo cmake
  emcmake cmake ..
fi
make
