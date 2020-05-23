#!/bin/bash

    g++ testinput.cpp -o testinput;
    g++ testprogram.cpp -o ref
    ./testinput < testinput.txt > out.txt;
    ./ref > ref.txt;
    diff ref.txt out.txt && echo "Success";