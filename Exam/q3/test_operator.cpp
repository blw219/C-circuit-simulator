#include "network.hpp"

using namespace std;

int main () {

    Network C_A = {'C', 0.5, {}};
    Network C_2 = {'C', 2, {}};
    Network R_A = {'R', 0.001, {}};
    Network L_A = {'L', 0.3, {}};
    Network L_10 = {'L', 10, {}};

    Network C_1 = {'C', 1, {}};
    Network R_1 = {'R', 1, {}};
    Network L_1 = {'L', 1, {}};

    Network RLC = {'|', 0, {C_1, L_1, R_1}};
    Network RLCC = {'|', 0, {L_1, C_1, R_1, C_1}};

    if (RLC < RLCC) {
        cout << "Success" << endl;
    }
}