#include <iostream>
#include <vector>
#include "tensor.hpp"
using namespace std;

int main(){
    //vector<int> x(9,10);
    //cout << x.size();
    vector<size_t> size={4,3,3,2},index;
    size_t a,b,c,d;
    cin >>a>>b>>c>>d;
    index ={a,b,c,d};
    cout << "size:[" << size[0] << ',' << size [1] << ',' << size [2] << ','<< size [3]<<']'<<endl;
    cout << "index:[" << index[0] << ',' << index[1] << ','<< index[2]<< ','<< index[3]<<']'<<endl;
    cout << "offset:" << index_to_offset(size,index,0) << endl;
    vector<size_t> res = offset_to_index(size,index_to_offset(size,index,0));
    cout << "(res)index:[";
    for(int i=0;i<res.size();i++){
        cout<< res[i] <<',';
    }
    cout<<']' << endl;
}