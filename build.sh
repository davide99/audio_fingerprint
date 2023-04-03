#!/usr/bin/env bash

#Su che SO sto compilando?
unameOut="$(uname -s)"
case "${unameOut}" in
  Linux*)     machine=Linux;;
  MINGW64*)   machine=MinGw64;;
  *)          machine="UNKNOWN"
esac

if [[ $machine == UNKNOWN ]]; then
  echo Build su questa architettura non supportata: usare Linux o MINGW64
  exit -1
fi

#Inizializzo SDK emscripten
if [[ $machine == Linux ]]; then
  source "$HOME"/emsdk/emsdk_env.sh
else
  source "$USERPROFILE"/emsdk/emsdk_env.sh
fi

#Creo, se non esiste già, cartella di build
BUILD_DIR="build"
mkdir -p $BUILD_DIR
cd $BUILD_DIR || exit

#Build
if [[ $1 == update ]]; then
  echo Rieseguo cmake
  #cmake dev'essere installato da apt, a meno di non mettere il percorso completo che punta
  #a quello bundled in clion
  emcmake cmake -DBUILD_CLIENT:BOOL=ON -DBUILD_SERVER:BOOL=OFF -DBUILD_MOCK_CLIENT:BOOL=OFF ..
fi

if [[ $machine == Linux ]]; then
  make
else
  mingw32-make
fi
