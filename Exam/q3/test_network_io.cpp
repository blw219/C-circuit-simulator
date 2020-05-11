#include "network.hpp"

bool equivalent(const Network &ref, const Network &got)
{
    if(ref.type != got.type){
        return false;
    }
    if(is_primitive(ref)){
        float diff=abs( (ref.value-got.value) / ref.value );
        if(diff > 1e-3){ // Add a little safety for 4 significant figures
            cerr<<"D Val, ref="<<ref.value<<", got="<<got.value<<endl;
            return false;
        }
    }else{
        if(ref.parts.size()!=got.parts.size()){
            cerr<<"D parts size, ref="<<ref.parts.size()<<", got="<<got.parts.size()<<endl;
            return false;
        }
        for(int i=0; i<ref.parts.size(); i++){
            if(!equivalent(ref.parts[i], got.parts[i])){
                return false;
            }
        }
    }
    return true;
}

int main()
{
    int fail=0;
    vector<Network> cases=create_test_networks();
    for(unsigned i=0; i<cases.size(); i++){
        Network ref=cases[i];

        stringstream tmp;
        tmp << cases[i];
        string rep=tmp.str();

        tmp.str(rep);
        Network got;
        tmp >> got;

        if(equivalent(ref,got)){
            cerr<<i<<" ok"<<endl;
        }else{
            cerr<<i<<" fail, rep="<<rep<<", roundtrip="<<got<<endl;
            fail=1;
        }
    }
    return fail;
}
