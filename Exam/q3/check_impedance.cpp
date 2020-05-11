#include "network.hpp" 
#include <complex>

int main () {

    /*
    R(1);
    C(1);
    L(1);
    Network RC = {'&', 0, {R(1), C(1)}}; //series RC
    Network CC = {'&',0, {C(1),C(2)}}; //parallel 2 capacitors
    Network LC_ser = {'&', 0 , {L(1),C(1)}}; //series LC
    Network LC_par = {'|', 0 , {L(1),C(1)}}; //parallel LC

    float omega = 0.5;

    complex<float> R_imp = impedance(R(1), omega);
    cout << R_imp.real()<< " " << R_imp.imag() <<"j" << endl;

    complex<float> C_imp = impedance(C(1), omega);
    cout << C_imp.real() << " " << C_imp.imag() <<"j" << endl;

    complex<float> L_imp = impedance(L(1), omega);
    cout << L_imp.real() << " " << L_imp.imag() <<"j" << endl;

    complex<float> RC_imp = impedance(RC, omega);
    cout << RC_imp.real() << " " << RC_imp.imag() <<"j" << endl;

    complex<float> CC_imp = impedance(CC, omega);
    cout << CC_imp.real() << " " << CC_imp.imag() <<"j" << endl;

    complex<float> LC_ser_imp = impedance(LC_ser, omega);
    cout << LC_ser_imp.real() << " " << LC_ser_imp.imag() <<"j" << endl;

    complex<float> LC_par_imp = impedance(LC_par, omega);
    cout << LC_par_imp.real() << " " << LC_par_imp.imag() <<"j" << endl;
    */

    Network R1_3 = {'R', 1.3, {}};
    Network C1_2 = {'C', 1.2, {}};
    Network L2 = {'L', 2, {}};
    Network L0_5 = {'L', 0.5, {}};
    Network R5 = {'R', 5, {}};
    Network C3 = {'C', 3, {}};

    Network subcom = {'|', 0, {R1_3, C1_2}};
    Network comb = {'|', 0, {subcom, L2, L0_5}};


}