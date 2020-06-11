#ifndef circuit2_hpp
#define circuit2_hpp

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include <fstream>
#include "eigen-3.3.7/Eigen/Dense"

using namespace std;

class component 
{
    public:
    char type;
    string nodep;
    string nodem;
    double value;
    string input_function;
    double amplitude;
    double frequency;
    double DC_offset;
    int identifier;
    string nodey;
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

    //returns the Thevenin voltage between a component in the circuit
    double find_thevenin_voltage(component x);

    //returns the Thevenin resistance between a component in the circuit
    double find_thevenin_resistance(component x);

    //do an operating point simulation for circuits without capacitors or inductors
    void simple_op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector);

    //do an operating point simulation/DC simulation of the circuit, then outputs node voltages
    void op_simulate();
    void op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector);

    //do a transient simulation of the circuit, then outputs node voltages in a .csv format
    void trans_simulate(double stoptime, double timestep);
};

istream &operator>>(istream &src, component &in);

#endif