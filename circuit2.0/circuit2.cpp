#include "circuit2.hpp"

vector<string> circuit::find_nodes()
{
    assert(!this->comps.empty());
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

void circuit::op_simulate(Eigen::MatrixXd conductance_matrix, Eigen::MatrixXd current_vector, Eigen::MatrixXd voltage_vector)
{
    assert(!comps.empty());
        //check if inductor is present in circuit
        vector<string> rm_nodes;
        vector<string> new_nodes;
        bool inductor_present = false;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'l'){
                rm_nodes.push_back(comps[i].nodem);
                new_nodes.push_back(comps[i].nodep);
                comps[i].nodem = comps[i].nodep;
                for(int j=0; j<comps.size(); j++){
                    for(int k=0; k<rm_nodes.size(); k++){
                        if(comps[j].nodep == rm_nodes[k]){
                            comps[j].nodep = new_nodes[k];
                        }
                        if(comps[j].nodem == rm_nodes[k]){
                            comps[j].nodem = new_nodes[k];
                        }
                    }
                }
            }
            inductor_present = true;
        }

        //beginning of computation
        vector<string> nodes = this->find_nodes();
        /*for(int i=0; i<nodes.size(); i++){
            cout << nodes[i] << endl;
        }*/
        int rows = nodes.size()-1;
        
        current_vector.setZero(rows, 1);
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
        voltage_vector = conductance_matrix.inverse() * current_vector;

        if(inductor_present){
            vector<int> present_nodes;
            for(int i=1; i<nodes.size(); i++){
                string tmp = nodes[i];
                tmp.erase(tmp.begin());
                present_nodes.push_back(stoi(tmp));
            }
           
            vector<int> insert_nodes;
            for(int i=0; i<rm_nodes.size(); i++){
                string tmp = rm_nodes[i];
                tmp.erase(tmp.begin());
                insert_nodes.push_back(stoi(tmp));
            }
            Eigen::MatrixXd new_voltage_vector;
            new_voltage_vector.setZero(voltage_vector.rows()+insert_nodes.size(),1);

            for(int i=0; i<voltage_vector.rows(); i++){
                new_voltage_vector(present_nodes[i]-1,0) = voltage_vector(i,0);
            }

            for(int i=0; i<insert_nodes.size(); i++){
                new_voltage_vector(insert_nodes[i]-1,0) = new_voltage_vector(insert_nodes[i]-2,0);
            }

            cout << "The voltage vector is: " << endl << new_voltage_vector << endl;

        }else{
            cout << "The voltage vector is: " << endl << voltage_vector << endl;
        } 
}

void circuit::trans_simulate(double stoptime, double timestep)
{
    assert(!comps.empty());

    double capacitor_current = 0;
    double inductor_voltage = 0;

    //op_simulate(); //include open circuit for capacitors and short circuit for inductors
    //store value of initial simulation in matrixes

    int counts = stoptime/timestep;
    for(int i=0; i<=counts; i++){
        //find current through every capacitor
        for(int j=0; j<=comps.size(); j++){
            if(comps[j].type == 'c'){
                //capacitor_current = voltage at (nodep - nodem) / (1/(2*pi*frequency))
                comps[j].type = 'v';
                //comps[j].value = capacitor_current*timestep/capacitance
            }

            if(comps[j].type == 'l'){
                //inductor_voltage = current through inductor * 2*pi*inductance
                comps[j].type = 'i';
                //comps[j].value = inductor_voltage*timestep/inductance
            }

        }

        //simulate
    }

}
