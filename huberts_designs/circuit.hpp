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
    #if type == 'Q'
        string nodey;
    #endif
    double value;
};

class circuit
{
    public:
    vector<component> comps;

    //returns a sorted vector of nodes in the circuit (with no duplicates)
    vector<string> find_nodes()
    {
        assert(!comps.empty());
        vector<string> nodes;
        for(int i=0; i<comps.size(); i++){
            nodes.push_back(comps[i].nodep);
            nodes.push_back(comps[i].nodem);
        }
        sort( nodes.begin(), nodes.end() );
        nodes.erase( unique( nodes.begin(), nodes.end() ), nodes.end() );
        return nodes;
    }

    //find components connected to a node
    vector<component> find_components(string node)
    {
        vector<component> x;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].nodep == node || comps[i].nodem == node){
                x.push_back(comps[i]);
            }
        }
        return x;
    }

    //finds components in between two nodes
    vector<component> find_components_between(string node1, string node2)
    {
        vector<component> x;
        for(int i=0; i<comps.size(); i++){
            if((comps[i].nodep == node1 && comps[i].nodem == node2)||(comps[i].nodep == node2 && comps[i].nodem == node1)){
                x.push_back(comps[i]);
            }
        }
        return x;
    }

    //do an operating point simulation/DC simulation of the circuit, then outputs node voltages
    void op_simulate()
    {
        assert(!comps.empty());
        vector<string> nodes = this->find_nodes();
        int rows = nodes.size()-1;

        //Finding the current vector
        Eigen::MatrixXd current_vector(rows, 1);
        for(int i=1; i<nodes.size(); i++){
            vector<component> a = this->find_components(nodes[i]);
            double tmp = 0;
            for(int j=0; j<a.size(); j++){
                if(a[j].type == 'I'){
                    if(a[j].nodep == nodes[i]){
                        tmp = tmp + a[j].value;
                    }
                    if(a[j].nodem == nodes[i]){
                        tmp = tmp - a[j].value;
                    }
                }
            }
            current_vector(i-1,0) = tmp;
        }
        cerr << "The current vector is: " << endl << current_vector << endl << endl;

        //Finding the conductance matrix
        Eigen::MatrixXd conductance_matrix(rows,rows);
        for(int i=1; i<nodes.size(); i++){
            //Finding Gii
            vector<component> a = this->find_components(nodes[i]);
            double tmp = 0;
            for(int j=0; j<a.size(); j++){
                if(a[j].type == 'R'){
                    tmp = tmp + 1/a[j].value;
                }
            }
            conductance_matrix(i-1,i-1) = tmp;

            //Finding other entries
            for(int j=1; j<nodes.size(); j++){
                if(i==j){
                    break;
                }
                //cout << "The nodes calculated are: " << nodes[i] << " " << nodes[j] << endl;
                vector<component> x = this->find_components_between(nodes[i],nodes[j]);
                double tmp = 0;
                for(int k=0; k<x.size(); k++){
                    if(x[k].type == 'R'){
                        tmp = tmp - 1/x[k].value;
                    }
                }
                conductance_matrix(j-1, i-1) = tmp;
                conductance_matrix(i-1, j-1) = tmp;
            }
        }
        cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        Eigen::MatrixXd voltage_vector = conductance_matrix.inverse() * current_vector;
        cout << "The voltage vector is: " << endl << voltage_vector << endl;
    }

    //do an operating point analysis with the inclusion of voltage sources
    void op_simulate_w_voltage()
    {
        assert(!comps.empty());
        vector<string> nodes = this->find_nodes();
        int rows = nodes.size()-1;
        
        Eigen::MatrixXd current_vector(rows, 1);
        Eigen::MatrixXd conductance_matrix(rows,rows);
        conductance_matrix.setZero(rows,rows);

        //Finding the current vector
        for(int i=1; i<nodes.size(); i++){
            vector<component> a = this->find_components(nodes[i]);
            double tmp = 0;
            for(int j=0; j<a.size(); j++){
                if(a[j].type == 'i'){
                    if(a[j].nodep == nodes[i]){
                        tmp = tmp + a[j].value;
                    }
                    if(a[j].nodem == nodes[i]){
                        tmp = tmp - a[j].value;
                    }
                }
                if(a[j].type == 'v'){
                    if(a[j].nodem == "0"){
                        tmp = tmp + a[j].value;
                        conductance_matrix(0,0) = 1;
                    }
                }
            }
            current_vector(i-1,0) = tmp;
        }
        cerr << "The current vector is: " << endl << current_vector << endl << endl;
        //cerr << conductance_matrix << endl;

        //Finding the conductance matrix
        for(int i=1; i<nodes.size(); i++){
            vector<component> a = this->find_components(nodes[i]);
            double tmp = 0;

            //Finding Gii
            for(int j=0; j<a.size(); j++){
                if(a[j].type == 'r'){
                    tmp = tmp + 1/a[j].value;
                }
            }
            conductance_matrix(i-1,i-1) = tmp;
            
    
            //Finding other entries
            for(int j=1; j<nodes.size(); j++){
                if(i==j){
                    break;
                }
                vector<component> x = this->find_components_between(nodes[i],nodes[j]);
                double tmp = 0;
                for(int k=0; k<x.size(); k++){
                    if(x[k].type == 'r'){
                        tmp = tmp - 1/x[k].value;
                    }
                }
                
                conductance_matrix(j-1, i-1) = tmp;
                conductance_matrix(i-1, j-1) = tmp;
            }
        }

        for(int i=0; i<this->comps.size(); i++){
            if(this->comps[i].type == 'v' && this->comps[i].nodem == "0"){
                string nod = this->comps[i].nodep;
                nod.erase(nod.begin());
                int pos = stoi(nod)-1;

                conductance_matrix(pos,pos) = 1;
                for(int j=0; j<conductance_matrix.cols(); j++){
                    if(conductance_matrix(pos,j) != 1){
                        conductance_matrix(pos,j) = 0;
                    }
                }
                break;
            }
        }
        cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        Eigen::MatrixXd voltage_vector = conductance_matrix.inverse() * current_vector;
        cout << "The voltage vector is: " << endl << voltage_vector << endl;
    }
};

istream &operator>>(istream &src, component &in){
    src >>in.type>>in.identifier>>in.nodep>>in.nodem>>in.value;
    if(src.fail()){
        cout << "Please input the correct node format" << endl;
    } 
    return src;
}