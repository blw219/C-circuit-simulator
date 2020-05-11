#include "network.hpp"

int main() {
    Network R1_3 = {'R', 1.3, {}};
    Network C1_2 = {'C', 1.2, {}};
    Network L2 = {'L', 2, {}};
    Network L0_5 = {'L', 0.5, {}};

    Network subcom = {'&', 0, {R1_3, C1_2}};
    Network comb = {'&', 0, {subcom, L2, L0_5}};

    cerr << "check if is_primitive is correct. return 1 for primitives, 0 for non-primitives." << endl;

    cerr << R1_3.type << is_primitive(R1_3) << endl;
    cerr << C1_2.type << is_primitive(C1_2) << endl;
    cerr << L2.type << is_primitive(L2) << endl;
    cerr << L0_5.type << is_primitive(L0_5) << endl;
    cerr << subcom.type << is_primitive(subcom) << endl;
    cerr << comb.type << is_primitive(comb) << endl;
    

}

