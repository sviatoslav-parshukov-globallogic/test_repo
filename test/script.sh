#!/bin/sh
apt-get update && apt-get upgrade
pwd
#cd
ls -l
apt-get install cmake -y
rm -rf build
mkdir build 
cd build
cmake ..
make