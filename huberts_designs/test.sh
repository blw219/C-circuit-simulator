#!/bin/bash

set -e

    g++ testinput.cpp -o testinput;
    
    ./testinput < testSPICE.txt > outSPICE.txt;
 
