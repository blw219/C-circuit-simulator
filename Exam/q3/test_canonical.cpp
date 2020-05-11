#include "network.hpp"

int main() {
    Network R1_3 = {'R', 1.3, {}};
    Network C1_2 = {'C', 1.2, {}};
    Network L2 = {'L', 2, {}};
    Network L0_5 = {'L', 0.5, {}};

    Network subcom = {'&', 0, {R1_3, C1_2}};
    Network comb = {'&', 0, {subcom, L2, L0_5}};
    Network res = canonicalise(comb);

    for (int i=0; i<res.parts.size(); i++) {
        cout << res.parts[i].value << endl;
    }
}