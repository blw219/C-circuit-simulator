#ifndef circuit4_hpp
#define circuit4_hpp

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
    string identifier;
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

    //do an operating point simulation for circuits without taking into account capacitors or inductors
    void op_simulate();
    void op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector, vector<double> &currents);

    void op_simple(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector, vector<double> &currents);

    //do a transient simulation of the circuit, then outputs node voltages in a .csv format
    void trans_simulate(double stoptime, double timestep);
};

#endif