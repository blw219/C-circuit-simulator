#ifndef circuit_hpp
#define circuit_hpp

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include "eigen-3.3.7/Eigen/Dense"

using namespace std;

class component
{
    public:
    char type;
    int identifier;
    string nodep;
    string nodem;
    string nodey;
    double value;
    string model;
};
 

class circuit
{
    public:
    vector<component> comps;

    //returns a sorted vector of nodes in the circuit (with no duplicates)
    vector<string> find_nodes();

    //find components connected to a node
    vector<component> find_components(string node);

    //finds components in between two nodes
    vector<component> find_components_between(string node1, string node2);

    //do an operating point simulation/DC simulation of the circuit, then outputs node voltages
    void op_simulate();

    //do an operating point analysis with the inclusion of voltage sources
    void op_simulate_w_voltage();

    void op_simulate_supernode();
};

istream &operator>>(istream &src, component &in);
ostream &operator<<(ostream &res, component &out);


#endif