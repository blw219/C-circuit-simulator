#include "network.hpp"

int main() {

    Network R1_3 = {'R', 1.3, {}};
    Network C1_2 = {'C', 1.2, {}};
    Network L2 = {'L', 2, {}};
    Network L0_5 = {'L', 0.5, {}};
    Network R5 = {'R', 5, {}};
    Network C3 = {'C', 3, {}};

    Network subcom = {'|', 0, {R1_3, C1_2}};
    Network comb = {'|', 0, {subcom, L2, L0_5}};

    Network subcom_sa = {'&', 0, {R1_3, C1_2}};
    Network subcom_sb = {'&', 0, {R5, C3}};
    Network comb_s = {'&', 0, {subcom_sa, subcom_sb, L2, L0_5}};

    Network comb_n = {'&', 0, {subcom, subcom_sa, L2, L0_5}};

    Network res_par = canonicalise(comb);
    Network res_ser = canonicalise(comb_s);

    Network res_noncan = canonicalise(comb_n);

    cout << endl << "parallel network" << endl;
    for (int i=0; i<res_par.parts.size(); i++) {
        cout << res_par.parts[i].type << res_par.parts[i].value << endl;
    }
    cout << res_par << endl;

    cout << endl << "series network" << endl;
    for (int i=0; i<res_ser.parts.size(); i++) {
        cout << res_ser.parts[i].type << res_ser.parts[i].value << endl;
    }
    cout << res_ser << endl;

    cout << endl << "this should not canonicalise properly" << endl;
    for (int i=0; i<res_noncan.parts.size(); i++) {
        cout << res_noncan.parts[i].type << res_noncan.parts[i].value << endl;
    }
    cout << res_noncan << endl;

}