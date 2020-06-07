#include "circuit.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include "eigen-3.3.7/Eigen/Dense"

vector<string> circuit::find_nodes()
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

vector<component> circuit::find_components(string node)
    {
        vector<component> x;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].nodep == node || comps[i].nodem == node){
                x.push_back(comps[i]);
            }
        }
        return x;
    }

vector<component> circuit::find_components_between(string node1, string node2)
    {
        vector<component> x;
        for(int i=0; i<comps.size(); i++){
            if((comps[i].nodep == node1 && comps[i].nodem == node2)||(comps[i].nodep == node2 && comps[i].nodem == node1)){
                x.push_back(comps[i]);
            }
        }
        return x;
    }

void circuit::op_simulate()
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

void circuit::op_simulate_w_voltage()
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
                if(a[j].type == 'I'){
                    if(a[j].nodep == nodes[i]){
                        tmp = tmp + a[j].value;
                    }
                    if(a[j].nodem == nodes[i]){
                        tmp = tmp - a[j].value;
                    }
                }
                if(a[j].type == 'V'){
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

        for(int i=0; i<this->comps.size(); i++){
            if(this->comps[i].type == 'V' && this->comps[i].nodem == "0"){
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


void circuit::op_simulate_supernode(){
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
            }
            current_vector(i-1,0) = tmp;

            for(int j=0; j<a.size(); j++){
                if(a[j].type == 'v' && current_vector(i-1,0) == 0){
                    double temp = a[j].value;
                    current_vector(i-1,0) = temp;
                }
            }
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
            //if voltage source is connected to GND
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
            }

            //if there is a supernode
            if(this->comps[i].type == 'v' && this->comps[i].nodem != "0"){
                string nod_plus = this->comps[i].nodep;
                nod_plus.erase(nod_plus.begin());
                int pos_plus = stoi(nod_plus)-1;

                string nod_min = this->comps[i].nodem;
                nod_min.erase(nod_min.begin());
                int pos_min = stoi(nod_min)-1;

                conductance_matrix(pos_plus,pos_plus) = 1;
                conductance_matrix(pos_plus, pos_min) = -1;
                conductance_matrix(pos_min, pos_plus) = 0;
                for(int j=0; j<conductance_matrix.cols(); j++){
                    if(!(conductance_matrix(pos_plus,j) == 1 || conductance_matrix(pos_plus,j) == -1)){
                        conductance_matrix(pos_plus,j) = 0;
                    }
                }
            }
        }
        cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        Eigen::MatrixXd voltage_vector = conductance_matrix.inverse() * current_vector;
        cout << "The voltage vector is: " << endl << voltage_vector << endl;
    }

    
istream &operator>>(istream &src, component &in){
    src>>in.type;
    if(in.type == 'V' || in.type == 'I' || in.type == 'R' || in.type == 'C' || in.type == 'L'){
         double value;
         char y[3];
         src>>in.identifier>>in.nodep>>in.nodem>>value;
         in.model = "N/A";
         in.nodey = "N/A";
         if(src.fail()){
             cout << "Please input the correct node format" << endl;
         }
         //if there is no multiplier, check ASCII, 10 = LINE FEED
         if(cin.peek()==10||cin.peek()==32 || cin.peek()==-1){
            in.value=value;
         }else if(cin.peek()==112){//p
            cin.get(y,2);
            in.value=(value/pow(10,12));
         }else if(cin.peek()==110){//n
            cin.get(y,2);
            in.value=(value/pow(10,9));
         }else if(cin.peek()==117){//u
            cin.get(y,2);
            in.value=(value/pow(10,6));
         }else if(cin.peek()==109){//m
            cin.get(y,2);
            in.value=(value/pow(10,3));
         }else if(cin.peek()==107){//k
            cin.get(y,2);
            in.value=(value*pow(10,3));
         }else if(cin.peek()==177){//M
            in.value=(value*pow(10,6));
            cin.get(y,4);
            if(y[1] != 'e' || y[2] != 'g'){
                cout << "We assumed you meant Meg, please check input value's multiplier" << endl;
            }
         }else if(cin.peek()==71){//G
            cin.get(y,2);
            in.value=(value*pow(10,9));
         }else{
            in.value= double(NAN);
            cerr << "unknown mutiplier";
         }
    }else if(in.type == 'D'){
         src>>in.identifier>>in.nodep>>in.nodem>>in.model;
         if(src.fail()){
             cout << "Please input the correct node format" << endl;
         }
         in.nodey = "N/A";
         in.value = double(NAN);
    }else if(in.type == 'Q'){
         src>>in.identifier>>in.nodep>>in.nodem>>in.nodey>>in.model;
         if(src.fail()){
             cout << "Please input the correct node format" << endl;
         }
         in.value = double(NAN);
         if(in.model!= "NPN" || in.model!= "PNP"){
            cerr << "Please enter the correct transistor model (NPN or PNP)";
         }
         assert (in.model=="NPN"||in.model=="PNP");
    }else{
        cout << "Please input the correct node format(refer to the technical report)" << endl;
    } 
    return src;
}

ostream &operator<<(ostream &res, component &out){
    if(out.type == 'V' ||out.type == 'I' ||out.type == 'R' ||out.type == 'C' ||out.type == 'L'){
        cout<< out.type << out.identifier <<' '<< out.nodep <<' '<< out.nodem <<' '<< out.value << endl;
    }else if(out.type == 'D'){
        cout<< out.type << out.identifier <<' '<< out.nodep <<' '<< out.nodem <<' '<< out.model << endl;
    }else if(out.type == 'Q'){
        cout<< out.type << out.identifier <<' '<< out.nodep <<' '<< out.nodem <<' '<< out.nodey<<' '<< out.model << endl;
    }else{
        cerr << ("output error");
    }
}
