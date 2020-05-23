#include "circuit.hpp"

int main()
{
    component R1 = {'r', "R1", "V1", "V2", 3};
    component R2 = {'r', "R2", "V1", "0", 4};
    component R3 = {'r', "R3", "V1", "V3", 6};
    component R4 = {'r', "R4", "V2", "0", 6};
    component R5 = {'r', "R5", "V2", "V3", 5};
    component R6 = {'r', "R6", "V3", "0", 8};
    component I1 = {'i', "I1", "V1", "0", 6};
    component I2 = {'i', "I2", "V3", "V1", 4};

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