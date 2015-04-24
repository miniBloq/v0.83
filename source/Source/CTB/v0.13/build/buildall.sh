#!/bin/bash

# Release without GPIB
make clean
make DEBUG=0 GPIB=0
sudo make DEBUG=0 GPIB=0 install
# Debug without GPIB
make clean
make DEBUG=1 GPIB=0
sudo make DEBUG=1 GPIB=0 install
# Release with GPIB
make clean
make DEBUG=0 GPIB=1
sudo make DEBUG=0 GPIB=1 install
# Debug with GPIB
make clean
make DEBUG=1 GPIB=1
sudo make DEBUG=1 GPIB=1 install
