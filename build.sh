#!/bin/bash
g++ main.cpp -std=c++17 -o card_reader_linux_x86 --static
aarch64-linux-gnu-g++ main.cpp -std=c++17 -o card_reader_arm --static
x86_64-w64-mingw32-g++ main.cpp -std=c++17 -o card_reader_windows_x64 --static