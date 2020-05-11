#include "network.hpp"

int main(int argc, char **argv)
{
    float a=1;
    float b=100;
    int n=10;

    if(argc>1){
        a=stof(argv[1]);
    }
    if(argc>2){
        b=stof(argv[2]);
    }
    if(argc>3){
        n=stoi(argv[3]);
    }

    Network n1, n2;
    cin >> n1 >> n2;

    vector<float> omega=make_log10_space(a, b, n);
    vector<complex<float>> H=transfer_function(n1, n2, omega);

    for(int i=0; i<H.size();i++){
        cout<<omega[i]<<", "<<abs(H[i])<<", "<<arg(H[i])<<endl;
    }
    //done
}
