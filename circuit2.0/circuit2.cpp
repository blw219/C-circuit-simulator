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

void circuit::simple_op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector)
{
    assert(!comps.empty());
        vector<string> nodes = this->find_nodes();
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
                if(a[j].type == 'v'){
                    if(a[j].nodem == "0"){
                        tmp = tmp + a[j].value;
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
        voltage_vector = conductance_matrix.inverse() * current_vector;
        cout << "The voltage vector is: " << endl << voltage_vector << endl;
}

void circuit::op_simulate()
{
    assert(!comps.empty());
        //check if inductor is present in circuit
        vector<string> rm_nodes;
        vector<string> new_nodes;
        bool inductor_present = false;
        bool voltage_present = false;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'l'){    
                if(comps[i].nodem == "0"){
                    rm_nodes.push_back(comps[i].nodep);
                    new_nodes.push_back(comps[i].nodem);
                    comps[i].nodep = "0";
                }else{
                    rm_nodes.push_back(comps[i].nodem);
                    new_nodes.push_back(comps[i].nodep);
                    comps[i].nodem = comps[i].nodep;
                } 
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
        int rows = nodes.size()-1;
        
        Eigen::MatrixXd current_vector;
        current_vector.setZero(rows, 1);
        Eigen::MatrixXd  conductance_matrix;
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
                    voltage_present = true;
                }
            }
        }
        cerr << "The current vector is: " << endl << current_vector << endl << endl;

        //Finding the conductance matrix
        if(conductance_matrix.rows() == 1 && voltage_present){
            conductance_matrix(0,0) = 1;
        }else{
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
                if(conductance_matrix.rows() > 1 && conductance_matrix.cols() > 1){
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
        }
        cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        Eigen::MatrixXd voltage_vector = conductance_matrix.inverse() * current_vector;

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
                if(tmp == "0"){
                    insert_nodes.push_back(stoi(tmp));
                }else{
                    tmp.erase(tmp.begin());
                    insert_nodes.push_back(stoi(tmp));
                }
            }
            Eigen::MatrixXd new_voltage_vector;
            new_voltage_vector.setZero(voltage_vector.rows()+insert_nodes.size(),1);

            //cerr << voltage_vector.rows()+insert_nodes.size() << "test" << endl;
            for(int i=0; i<voltage_vector.rows(); i++){
                new_voltage_vector(present_nodes[i]-1,0) = voltage_vector(i,0);
            }

            for(int i=0; i<insert_nodes.size(); i++){
                if(insert_nodes[i] == 0){
                    new_voltage_vector(0,0) = new_voltage_vector(1,0);
                    new_voltage_vector(1,0) = 0;
                }else{
                    new_voltage_vector(insert_nodes[i]-1,0) = new_voltage_vector(insert_nodes[i]-2,0);
                }
            }

            voltage_vector = new_voltage_vector;

            cout << "The voltage vector is: " << endl << voltage_vector << endl;

        }else{
            cout << "The voltage vector is: " << endl << voltage_vector << endl;
        }

        ofstream output_file;
        output_file.open("op_analysis_output.csv");

        output_file << "Node" << "," << "Voltage" << endl;

        for(int i=0; i<voltage_vector.rows(); i++){
            output_file << "N" << i+1 << "," << voltage_vector(i,0) << endl;
        }
}

void circuit::op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector)
{
    assert(!comps.empty());
        //check if inductor is present in circuit
        vector<string> rm_nodes;
        vector<string> new_nodes;
        bool inductor_present = false;
        bool voltage_present = false;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'l'){    
                if(comps[i].nodem == "0"){
                    rm_nodes.push_back(comps[i].nodep);
                    new_nodes.push_back(comps[i].nodem);
                    comps[i].nodep = "0";
                }else{
                    rm_nodes.push_back(comps[i].nodem);
                    new_nodes.push_back(comps[i].nodep);
                    comps[i].nodem = comps[i].nodep;
                } 
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
                    voltage_present = true;
                }
            }
        }
        //cerr << "The current vector is: " << endl << current_vector << endl << endl;
        if(conductance_matrix.rows() == 1 && voltage_present){
            conductance_matrix(0,0) = 1;
        }else{
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
                if(conductance_matrix.rows() > 1 && conductance_matrix.cols() > 1){
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
        }
        //Finding the conductance matrix
        
        //cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        voltage_vector = conductance_matrix.inverse() * current_vector;

        if(inductor_present && voltage_vector.rows()>1){
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

            voltage_vector = new_voltage_vector;

            //cout << "The voltage vector is: " << endl << voltage_vector << endl;

        }else{
            //cout << "The voltage vector is: " << endl << voltage_vector << endl;
        }
}

void circuit::trans_simulate(double stoptime, double timestep)
{
    assert(!comps.empty());

    const double pi = 3.141592654;
    circuit a = *this;

    double capacitor_voltage = 0;
    double inductor_current = 0;

    //store value of initial simulation in matrixes
    Eigen::MatrixXd conductance_matrix;
    Eigen::MatrixXd current_vector;
    Eigen::MatrixXd voltage_vector;
    a.simple_op_simulate(conductance_matrix, current_vector, voltage_vector);

    //finding 'Thevenin Resistance'
    double resistance;
    for(int i=0; i<a.comps.size(); i++){
        if(a.comps[i].type == 'r'){
            resistance = a.comps[i].value;
            break;
        }
    }

    int counts = stoptime/timestep;
    //finding the frequency and amplitude
    double frequency = 0;
    double amplitude = 0;
    for(int i=0; i<comps.size(); i++){
        if(comps[i].type == 'v' && comps[i].input_function == "AC"){
            frequency = comps[i].frequency;
            amplitude = comps[i].amplitude;
            break;
        }
    }

    //create output file
    ofstream out;
    out.open("transient_output.csv");
    out << "Time(s)";
    vector<string> nodes = this->find_nodes();
    for(int i=1; i<nodes.size(); i++){
        out << "," << "N" << i;
    }
    out << endl;

    for(int i=0; i<=counts; i++){
        //find current through every capacitor
        a = *this;
        for(int j=0; j<=a.comps.size(); j++){
            if(a.comps[j].type =='c'){
                string tmp_nodep = a.comps[j].nodep;
                //cerr << tmp_nodep << endl;
                int pos_p;
                if(tmp_nodep == "0"){
                    pos_p = 0;
                }else{
                    tmp_nodep.erase(tmp_nodep.begin());
                    pos_p = stoi(tmp_nodep)-1; 
                }

                string tmp_nodem = a.comps[j].nodem;
                //cerr << tmp_nodem << endl;
                int pos_m;
                if(tmp_nodem == "0"){
                    pos_m = 0;
                }else{
                    tmp_nodem.erase(tmp_nodem.begin());
                    pos_m = stoi(tmp_nodep)-1;
                }

                double capacitance = a.comps[j].value;
                a.comps[j].type = 'v';
                //cout << voltage_vector(pos_p,0) << endl << endl;
                double capacitor_current = (voltage_vector(0,0)-voltage_vector(pos_p,0))/resistance; //might need to be modified in the future
                //cout << capacitor_current << endl;
                capacitor_voltage += capacitor_current*timestep/capacitance;
                a.comps[j].value = capacitor_voltage;
                //cout << capacitor_voltage << endl << endl;
            }

            if(comps[j].type == 'l'){
                //inductor_voltage = voltage between inductor
                //comps[j].value = inductor_voltage*timestep/inductance
                string tmp_nodep = a.comps[j].nodep;
                //cerr << tmp_nodep << endl;
                int pos_p;
                double v_plus;
                if(tmp_nodep == "0"){
                    pos_p = 0;
                    v_plus = 0;
                }else{
                    tmp_nodep.erase(tmp_nodep.begin());
                    pos_p = stoi(tmp_nodep)-2; 
                    v_plus = voltage_vector(pos_p,0);
                }

                string tmp_nodem = a.comps[j].nodem;
                //cerr << tmp_nodem << endl;
                int pos_m;
                double v_min;
                if(tmp_nodem == "0"){
                    pos_m = 0;
                    v_min = 0;
                }else{
                    tmp_nodem.erase(tmp_nodem.begin());
                    pos_m = stoi(tmp_nodep)-1;
                    v_min = voltage_vector(pos_m,0);
                }

                double inductance = a.comps[j].value;
                a.comps[j].type = 'i';
                double inductor_voltage = (v_plus-0)/resistance; 
                inductor_current -= inductor_voltage*timestep/inductance;
                a.comps[j].value = inductor_current;
            }
        }

        for(int j=0; j<a.comps.size(); j++){
            if(a.comps[j].type == 'v' && a.comps[j].input_function == "AC"){
                a.comps[j].value = amplitude*sin(i*timestep*frequency*2*pi)+a.comps[j].DC_offset;
                break;
            }
        }
    
        //print to csv per row
        a.op_simulate(conductance_matrix,current_vector,voltage_vector);
        out << i*timestep;
        for(int j=0; j<voltage_vector.rows(); j++){
            out << "," << voltage_vector(j,0);
        }
        out << endl;   
    }
    cerr << "Transient Analysis Completed" << endl;
}
