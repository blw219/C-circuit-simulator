#include "circuit2.hpp"

using namespace std;

double suffix_converter(string x);

string getstring(char x)
{
    string s(1,x);
    return s;
}
vector<string> breakdown(string x)
{
    vector<string> a;
    int begin_pos = 0;
    for(int i=0; i<x.length(); i++){
        if(x[i] == ' ' || i == x.length()-1){
            int end_pos = i-1;
            string tmp;
            if(begin_pos == 0 && end_pos == 0){
                tmp = x[0];
            }else{
                tmp = x.substr(begin_pos, end_pos);
            }
            a.push_back(tmp);
            begin_pos = i+1;
        }
    }
    return a;
}

int main()
{
    ifstream file;
    file.open("inputnetlist.txt");

    if(!file.is_open()){
        cout << "Error: Input file does not exist." << endl;
        exit(1);
    }else{
        cout << "Reading from file..." << endl;
    }

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
            cout << "Conducting operating point analysis..." << endl;
            op_analysis = true;
        }

        if(lines[i][0] == '.' && lines[i][1] == 't'){
            cout << "Conducting transient analysis..." << endl;
            transient_analysis = true;
            vector<string> trans = breakdown(lines[i]);
            stop_time = stod(trans[2]);
            timestep = stod(trans[4]);
        }

        if(lines[i][0] == 'R' || lines[i][0] == 'L' || lines[i][0] == 'C' || lines[i][0] == 'V'|| lines[i][0] == 'I'){
            cout << "Component found: " << lines[i][0] << endl;
            vector<string> words = breakdown(lines[i]);
            component tmp;

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
                tmp.nodep = "V" + getstring(words[1][3]);
            }else{
                tmp.nodep = "0";
            }

            if(words[2][0] == 'N'){
                tmp.nodem = "V" + getstring(words[2][3]);
            }else{
                tmp.nodem = "0";
            }

            //finding values
            tmp.value = stod(words[3]);

            circ.push_back(tmp);
        }
    }

    for(int i=0; i<circ.size(); i++){
        cout << circ[i].type << " " << circ[i].nodep << " " << circ[i].nodem << " " << circ[i].value << endl; 
    }

    circuit final;
    final.comps = circ;

    //conducting analysis
    if(op_analysis){
        final.op_simulate();
    }

    if(transient_analysis){
        final.trans_simulate(stop_time,timestep);
    }
}