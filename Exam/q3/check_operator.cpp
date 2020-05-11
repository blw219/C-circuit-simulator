#include "network.hpp"

int main() {

    Network R1_3 = {'R', 1.3, {}};
    Network C1_2 = {'C', 1.2, {}};
    Network L2 = {'L', 2, {}};
    Network L0_5 = {'L', 0.5, {}};

    Network subcom = {'&', 0, {R1_3, C1_2}};
    Network comb = {'&', 0, {subcom, L2, L0_5}};

    cerr << "check if less than operator is correct. 1 if less-than operator is true. & < C < L < R < L " << endl;

    cerr << "R1_3 < C1_2 should be false: " << (R1_3 < C1_2) << endl;
    cerr << "C1_2 < R1_3 should be true: " << (C1_2 < R1_3) << endl;
    cerr << "L0_5 < L2 should be true: " << (L0_5 < L2) << endl;
    cerr << "comb < subcom should be false: " << (comb < subcom) << endl;

    cerr << endl << "check flattening circuits as well." << endl;

}