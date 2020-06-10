#!/bin/bash

set -e

    g++ testinput.cpp circuit.cpp -o testinput;
    g++ testprogram.cpp circuit.cpp -o ref;

    ./testinput < ./test_files/testSPICE1.txt > ./test_files/outSPICE1.txt &>> ./test_files/outSPICE1.txt &>> ./test_files/outSPICE1.txt;
    ./ref > ./test_files/ref.txt &>> ./test_files/ref.txt &>> ./test_files/ref.txt;
    diff ./test_files/ref.txt ./test_files/outSPICE1.txt && echo "Success";