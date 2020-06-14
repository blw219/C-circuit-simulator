#include "circuit4.hpp"

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

void circuit::op_simulate()
{
    assert(!comps.empty());
        vector<string> nodes = this->find_nodes();
        int rows = nodes.size()-1;
        bool voltage_present = false;

        double total_voltage = 0;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'v'){
                total_voltage += comps[i].value;
            }
            if(comps[i].type == 'l'){
                comps[i].type = 'r';
                comps[i].value = 0;
            }
            if(comps[i].type == 'c'){
                comps[i].type = 'r';
                comps[i].value = 999999999999;
            }
        }
        
        Eigen::MatrixXd conductance_matrix;
        Eigen::MatrixXd current_vector;
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
        for(int i=0; i<voltage_vector.rows(); i++){
            double temp = voltage_vector(i,0);
            if(isnan(temp)){
                voltage_vector(i,0) = total_voltage;
            }
        }

        //Creating output file
        ofstream out;
        ifstream file;
        file.open("op_output.txt");
        if(file.is_open()){
            remove("op_output.txt");
        }
        out.open("op_output.txt");
        //cerr << "The voltage vector is: " << endl << voltage_vector << endl;
        out << "The voltage vector is: " << endl << voltage_vector << endl;

        //cerr << endl << "The currents are: " << endl;
        out << endl << "The currents are: " << endl;
        
        //vector<int> inductor_positions;
        vector<double> currents;
        double current;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'r'){
                double resistance = comps[i].value;
                string p = comps[i].nodep;
                int posp;
                bool pzero = false;
                if(p == "0"){
                    pzero = true;
                }else{
                    p.erase(p.begin());
                    posp = stoi(p)-1;
                }
                string m = comps[i].nodem;
                int posm;
                bool mzero = false;
                if(m == "0"){
                    mzero = true;
                }else{
                    m.erase(m.begin());
                    posm = stoi(m)-1;
                }

                if(pzero){
                    current = -voltage_vector(posm,0)/resistance;
                }else if(mzero){
                    current = voltage_vector(posp,0)/resistance;
                }else{
                    current = (voltage_vector(posp,0)-voltage_vector(posm,0))/resistance;
                }
                out << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'l'){
                current = 0;
                out << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'c'){
                current = 0;
                out << current << endl;
                currents.push_back(current);
            }
        }
}

void circuit::op_simulate(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector, vector<double> &currents)
{
    assert(!comps.empty());
        currents.clear();
        vector<string> nodes = this->find_nodes();
        int rows = nodes.size()-1;
        bool voltage_present = false;
        
        double total_voltage = 0;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'v'){
                total_voltage += comps[i].value;
            }
        }
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
        //cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        voltage_vector = conductance_matrix.inverse() * current_vector;
        for(int i=0; i<voltage_vector.rows(); i++){
            double temp = voltage_vector(i,0);
            if(isnan(temp)){
                voltage_vector(i,0) = total_voltage;
            }
        }
        //cout << "The voltage vector is: " << endl << voltage_vector << endl;

        //cout << endl << "The currents are: " << endl;
        
        //vector<int> inductor_positions;
        double current;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'r'){
                double resistance = comps[i].value;
                string p = comps[i].nodep;
                int posp;
                bool pzero = false;
                if(p == "0"){
                    pzero = true;
                }else{
                    p.erase(p.begin());
                    posp = stoi(p)-1;
                }
                string m = comps[i].nodem;
                int posm;
                bool mzero = false;
                if(m == "0"){
                    mzero = true;
                }else{
                    m.erase(m.begin());
                    posm = stoi(m)-1;
                }

                if(pzero){
                    current = -voltage_vector(posm,0)/resistance;
                }else if(mzero){
                    current = voltage_vector(posp,0)/resistance;
                }else{
                    current = (voltage_vector(posp,0)-voltage_vector(posm,0))/resistance;
                }
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'l'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'c'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'v'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'i'){
                current = comps[i].value;
                //cout << current << endl;
                currents.push_back(current);
            }
        }
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'v'){
                string p = comps[i].nodep;
                current = 0;
                for(int j=0; j<comps.size(); j++){
                    if(p == comps[j].nodep){
                        current -= currents[j];
                    }
                    if(p == comps[j].nodem){
                        current += currents[j];
                    }
                }
                //cout << current << endl;
                currents[i] = current;
            }
        }
}

void circuit::op_simple(Eigen::MatrixXd &conductance_matrix, Eigen::MatrixXd &current_vector, Eigen::MatrixXd &voltage_vector, vector<double> &currents)
{
    assert(!comps.empty());
        currents.clear();
        vector<string> nodes = this->find_nodes();
        int rows = nodes.size()-1;
        bool voltage_present = false;
        
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
        //cerr << "The conductance matrix is: " << endl << conductance_matrix << endl << endl;

        //Calculate the voltage vector
        voltage_vector = conductance_matrix.inverse() * current_vector;
        //cout << "The voltage vector is: " << endl << voltage_vector << endl;

        //cout << endl << "The currents are: " << endl;
        
        //vector<int> inductor_positions;
        double current;
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'r'){
                double resistance = comps[i].value;
                string p = comps[i].nodep;
                int posp;
                bool pzero = false;
                if(p == "0"){
                    pzero = true;
                }else{
                    p.erase(p.begin());
                    posp = stoi(p)-1;
                }
                string m = comps[i].nodem;
                int posm;
                bool mzero = false;
                if(m == "0"){
                    mzero = true;
                }else{
                    m.erase(m.begin());
                    posm = stoi(m)-1;
                }

                if(pzero){
                    current = -voltage_vector(posm,0)/resistance;
                }else if(mzero){
                    current = voltage_vector(posp,0)/resistance;
                }else{
                    current = (voltage_vector(posp,0)-voltage_vector(posm,0))/resistance;
                }
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'l'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'c'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'v'){
                current = 0;
                //cout << current << endl;
                currents.push_back(current);
            }
            if(comps[i].type == 'i'){
                current = comps[i].value;
                //cout << current << endl;
                currents.push_back(current);
            }
        }
        for(int i=0; i<comps.size(); i++){
            if(comps[i].type == 'v'){
                string p = comps[i].nodep;
                current = 0;
                for(int j=0; j<comps.size(); j++){
                    if(p == comps[j].nodep){
                        current -= currents[j];
                    }
                    if(p == comps[j].nodem){
                        current += currents[j];
                    }
                }
                //cout << current << endl;
                currents[i] = current;
            }
        }
}

void circuit::trans_simulate(double stoptime, double timestep)
{
    assert(!comps.empty());

    const double pi = 3.141592654;
    circuit a = *this;

    vector<double> cap_voltages;
    vector<double> ind_currents;
    vector<string> identifiers;

    for(int i=0; i<comps.size(); i++){
        if(comps[i].type == 'c'){
            cap_voltages.push_back(0);
        }
        if(comps[i].type == 'l'){
            ind_currents.push_back(0);
        }
        identifiers.push_back(comps[i].identifier);
    }



    //store value of initial simulation in matrixes
    Eigen::MatrixXd conductance_matrix;
    Eigen::MatrixXd current_vector;
    Eigen::MatrixXd voltage_vector;
    vector<double> currents;
    a.op_simulate(conductance_matrix, current_vector, voltage_vector, currents);

    /*for(int i=0; i<currents.size(); i++){
        cerr << currents[i] << endl;
    }*/

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
        out << "," << "Node" << i << " (V)";
    }
    for(int i=0; i<identifiers.size(); i++){
        out << "," << identifiers[i] << " current (A)";
    }
    out << endl;

    for(int i=0; i<=counts; i++){
        //find voltage of every capacitor
        a = *this;
        /*for(int j=0; j<currents.size(); j++){
            cerr << currents[j] << endl;
        }
        cerr << endl;*/
        int cap_it = 0;
        int ind_it = 0;
        for(int j=0; j<=a.comps.size(); j++){
            if(a.comps[j].type =='c'){
                string tmp_nodep = a.comps[j].nodep;
                string tmp_nodem = a.comps[j].nodem;

                double capacitor_current = 0;
                double capacitance = a.comps[j].value;

                a.comps[j].type = 'v';
                //cout << voltage_vector(pos_p,0) << endl << endl;
                for(int k=0; k<a.comps.size(); k++){
                    if(a.comps[k].nodem == tmp_nodep){
                        //cerr << a.comps[k].type << endl;
                        //cerr << k << endl;
                        capacitor_current = currents[k];
                        //cerr << capacitor_current << endl;
                    }
                }
                //cout << capacitor_current << endl;
                //cerr << cap_it << endl;
                double &capacitor_voltage = cap_voltages[cap_it];
                capacitor_voltage += capacitor_current*timestep/capacitance;
                a.comps[j].value = capacitor_voltage;
                cap_it++;
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
                    pos_p = stoi(tmp_nodep)-1; 
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
                    pos_m = stoi(tmp_nodem)-1;
                    v_min = voltage_vector(pos_m,0);
                }

                double inductance = a.comps[j].value;
                a.comps[j].type = 'i';
                double inductor_voltage;
                //cerr << v_plus << " " << v_min << endl;
                inductor_voltage = v_plus-v_min;
                double &inductor_current = ind_currents[ind_it];
                inductor_current -= inductor_voltage*timestep/inductance;
                //cerr << inductor_current << endl;
                a.comps[j].value = inductor_current;
                ind_it++;
            }
        }

        for(int j=0; j<a.comps.size(); j++){
            if(a.comps[j].type == 'v' && a.comps[j].input_function == "AC"){
                a.comps[j].value = amplitude*sin(i*timestep*frequency*2*pi)+a.comps[j].DC_offset;
                break;
            }
        }
    
        //print to csv per row
        a.op_simple(conductance_matrix,current_vector,voltage_vector, currents);
        
        out << i*timestep;
        for(int j=0; j<voltage_vector.rows(); j++){
            out << "," << voltage_vector(j,0);
        }
        for(int j=0; j<currents.size(); j++){
            out << "," << currents[j];
        }
        out << endl;   
    }
    cerr << "Transient Analysis Completed" << endl;
}