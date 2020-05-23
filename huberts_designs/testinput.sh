#!/bin/bash

    g++ testinput.cpp -o testinput;
    g++ testprogram.cpp -o ref
    ./testinput < testSPICE.txt > outSPICE.txt;
    ./ref > ref.txt;
    diff ref.txt outSPICE.txt && echo "Success";