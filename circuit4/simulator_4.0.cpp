#include "circuit4.hpp"
#include <ctime>
using namespace std;

double suffix_converter(string x)
{
    string number = x;
    double multiplier = 1;
    for(int i=0; i<x.length(); i++){
        if(x[i] == 'p' || x[i] == 'n' || x[i] == 'u' || x[i] == 'm' || x[i] == 'k' || x[i] == 'M' || x[i] == 'G'){
            number = x.substr(0,i);
        }
        if(x[i] == 'p'){
            multiplier = 1e-12;
        }
        if(x[i] == 'n'){
            multiplier = 1e-9;
        }
        if(x[i] == 'u'){
            multiplier = 1e-6;
        }
        if(x[i] == 'm'){
            multiplier = 1e-3;
        }
        if(x[i] == 'k'){
            multiplier = 1e3;
        }
        if(x[i] == 'M'){
            multiplier = 1e6;
        }
        if(x[i] == 'G'){
            multiplier = 1e9;
        }
    }
    double no = stod(number);
    double final = no * multiplier;
    return final;
}

string getstring(char x)
{
    string s(1,x);
    return s;
}

vector<string> breakdown(string x)
{   
    vector<string> words;
    int begin = 0;
    for(int i=0; i<x.length(); i++){
        if(x[i] == ' '){
            int length = i-begin;
            string tmp = x.substr(begin, length);
            words.push_back(tmp);
            begin = i+1;
        }else if(i == x.length()-1){
            int length = i-begin+1;
            string tmp = x.substr(begin, length);
            words.push_back(tmp);
            begin = i+1;
        }
    }
    return words;
}

int main()
{
    string inputfile;
    ifstream file;
    //Locating input file
    cout << "Please enter name of input file:" << endl;
    cin >> inputfile;
    if(inputfile == "exit"){
            cout << "Program killed" << endl;
            return 1;
    }
    time_t start, finish;
    time(&start);
    file.open(inputfile);
    
    while(!file.is_open()){
        cout << "Error: Input file does not exist." << endl;
        cout << "Please enter name of input file or enter 'exit' to kill the program" << endl;
        cin >> inputfile;
        if(inputfile == "exit"){
            cout << "Program killed" << endl;
            exit(1);
        }else{
        file.open(inputfile);
        }
    }
        cout << "Reading from " << inputfile << " ..." << endl;
    

    vector<string> lines;
    string line;
    while(file.good()){
        getline(file, line);
        lines.push_back(line);
    }

    vector<component> circ;
    bool op_analysis = false;
    bool transient_analysis = false;
    double stop_time = 0;
    double timestep = 0;
    for(int i=0; i<lines.size(); i++){
        if(lines[i][0] == '*'){
            cout << "Comment found: Comment is ignored" << endl;
        }

        if(lines[i] == ".end"){
            cout << "End of netlist" << endl;
            break;
        }

        if(lines[i] == ".op"){
            cout << "Operating point analysis selected..." << endl;
            op_analysis = true;
        }

        if(lines[i][0] == '.' && lines[i][1] == 't'){
            cout << "Transient analysis selected..." << endl;
            transient_analysis = true;
            vector<string> trans = breakdown(lines[i]);
            //cerr << trans[2] << endl;
            stop_time = suffix_converter(trans[2]);
            timestep = suffix_converter(trans[4]);
            cout << "Stop Time: " << stop_time << endl;
            cout << "Time Step: " << timestep << endl;
        }

        if(lines[i][0] == 'R' || lines[i][0] == 'L' || lines[i][0] == 'C' || lines[i][0] == 'V'|| lines[i][0] == 'I'){
            cout << "Component found: " << lines[i][0] << endl;
            vector<string> words = breakdown(lines[i]);
            component tmp;
            tmp.identifier =  words[0];
            //finding the type of component
            if(lines[i][0] == 'R'){
                tmp.type = 'r';
            }
            if(lines[i][0] == 'L'){
                tmp.type = 'l';
            }
            if(lines[i][0] == 'C'){
                tmp.type = 'c';
            }
            if(lines[i][0] == 'V'){
                tmp.type = 'v';
            }
            if(lines[i][0] == 'I'){
                tmp.type = 'i';
            }

            //finding the nodes
            if(words[1][0] == 'N'){
                string str = words[1];
                str.erase(str.begin());
                int ind = stoi(str);
                tmp.nodep = "V" + to_string(ind);
            }else{
                tmp.nodep = "0";
            }

            if(words[2][0] == 'N'){
                string str = words[2];
                str.erase(str.begin());
                int ind = stoi(str);
                tmp.nodem = "V" + to_string(ind);
            }else{
                tmp.nodem = "0";
            }

            //finding values
            if(words[3][0] == 'S'){
                string str = words[3];
                str.erase(0,5);
                double offset = stod(str);
                string strr = words[5];
                strr.erase(strr.end());
                tmp.DC_offset = offset;
                tmp.amplitude = stod(words[4]);
                tmp.frequency = stod(strr);
                tmp.input_function = "AC";
                tmp.value = offset;
            }else{
                tmp.value = suffix_converter(words[3]);
            }
            circ.push_back(tmp);
        }
    }

    cout << "The circuit consists of these components: " << endl;
    for(int i=0; i<circ.size(); i++){
        cout << circ[i].type << " " << circ[i].nodep << " " << circ[i].nodem << " " << circ[i].value << endl; 
    }

    cout << "End of circuit." << endl;

    circuit final;
    final.comps = circ;

    //conducting analysis
    if(op_analysis){
        cerr << "Warning: circuits with capacitors and current sources" << endl;
        final.op_simulate();
        time(&finish);
        cerr << "Time taken: " << fixed << setprecision(5) << difftime(finish,start)  << "seconds" << endl;
    }

    if(transient_analysis){
        final.trans_simulate(stop_time,timestep);
        time(&finish);
        cerr << "Time taken: " << fixed  << difftime(finish,start)  << setprecision(5) << "seconds" << endl;
    }else if(!op_analysis){
        cerr << "Error: transient analysis directive not deteced." << endl;
    }
}