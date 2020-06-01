#ifndef circuit2_hpp
#define circuit2_hpp

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
    string nodep;
    string nodem;
    double value;
    
};

class AC_source
    : public component
{
    public: 
    string input_function;
    float amplitude;
    float frequency;
    float DC_offset;
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
    void op_simulate(Eigen::MatrixXd conductance_matrix, Eigen::MatrixXd current_vector, Eigen::MatrixXd voltage_vector);

    //do a transient simulation of the circuit, then outputs node voltages in a .csv format
    void trans_simulate(double stoptime, double timestep);
};

#endif