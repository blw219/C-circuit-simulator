#!/bin/bash

set -e

    g++ testinput.cpp circuit.cpp -o testinput;
    g++ testprogram.cpp circuit.cpp -o ref;

    ./testinput < ./test_files/testSPICE.txt > ./test_files/outSPICE.txt &>> ./test_files/outSPICE.txt &>> ./test_files/outSPICE.txt;
    ./ref > ./test_files/ref.txt &>> ./test_files/ref.txt >> ./test_files/ref.txt;
    diff ref.txt outSPICE.txt && echo "Success";