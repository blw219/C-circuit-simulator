#!/bin/bash

set -e

    g++ testinput.cpp circuit.cpp -o testinput;
    g++ testprogram.cpp circuit.cpp -o ref;

    ./testinput < testSPICE.txt > outSPICE.txt;
    ./ref > ref.txt;
    diff ref.txt outSPICE.txt && echo "Success";