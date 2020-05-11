#include "network.hpp"

void print_parts(const Network &n)
{
    if(is_primitive(n)){
        cout << n.type << n.value << endl;
    }else{
        for(unsigned i=0; i<n.parts.size(); i++){
            print_parts(n.parts[i]);
        }
    }
}

int main()
{
    Network n;

    cin >> n;

    print_parts(n);
}
