#include "circuit.hpp"

int main()
{
    component R1 = {'R', 1, "N1", "N2", 3};
    component R2 = {'R', 2, "N1", "0", 4};
    component R3 = {'R', 3, "N1", "N3", 6};
    component R4 = {'R', 4, "N2", "0", 6};
    component R5 = {'R', 5, "N2", "N3", 5};
    component R6 = {'R', 6, "N3", "0", 8};
    component I1 = {'I', 1, "N1", "0", 6};
    component I2 = {'I', 2, "N3", "N1", 4};

    circuit a;
    a.comps.push_back(R1);
    a.comps.push_back(R2);
    a.comps.push_back(R3);
    a.comps.push_back(R4);
    a.comps.push_back(R5);
    a.comps.push_back(R6);
    a.comps.push_back(I1);
    a.comps.push_back(I2);

    /*vector<component> x = a.find_components_between("V1", "V3");

    for(int i=0; i<x.size(); i++){
        cout << x[i].type << " " << x[i].value << endl;
    }*/
    a.op_simulate();
    /*vector<component> v = a.find_components("V1");
    for(int i=0; i<v.size(); i++){
        cout << v[i].type << " " << v[i].value << endl;
    }*/
}