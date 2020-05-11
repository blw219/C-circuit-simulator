#include "network.hpp"

int main()
{
  vector<Network> printer = create_test_networks();
  for(int i=0; i<printer.size(); i++){
    cout << printer[i].type << endl;
  }
}
