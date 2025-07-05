@echo off
mkdir build
cd build
wsl cmake ..
wsl cmake --build .
cd ..
wsl ./build/tradesim
