#!/usr/bin/sh
apt-get update && apt-get upgrade
apt-get install cmake -y
ls -l
rm -rf build
mkdir build 
cd build
cmake ../
make