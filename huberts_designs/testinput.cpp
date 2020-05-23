#include "circuit.hpp"

void print(component &R1){
    cout<<R1.type<<R1.identifier;//<<" "<<R1.nodep<<" "<<R1.nodem<<" "<<R1.value<<endl;
}

int main()
{
    component R1, R2, R3, R4, R5, R6, I1, I2;
    cin>>R1>>R2>>R3>>R4>>R5>>R6>>I1>>I2;
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