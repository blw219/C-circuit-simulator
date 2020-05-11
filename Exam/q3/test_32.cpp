#include "network.hpp"

int main()
{
  vector<float> omega = {1, 2, 4};
  vector<complex<float>> printer = transfer_function(R(2), C(3), omega);
  for(int i=0; i<printer.size(); i++){
    cout << printer[i].real() << " " << printer[i].imag() << "j" << endl;
  }
}
