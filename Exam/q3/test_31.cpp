#include "network.hpp"

int main()
{
  vector<Network> a = {R(1), C(1), L(1), {'&', 0, {R(1), C(1)}}, {'&', 0, {C(1), C(2)}}};
  for(int i=0; i<a.size(); i++){
    complex<float> imp_half = impedance(a[i], 0.5);
    complex<float> imp_one = impedance(a[i], 1);
    complex<float> imp_two = impedance(a[i], 2);

    cout << a[i] << endl;
    cout << imp_half.real() << " + " << imp_half.imag() << "j, " << imp_one.real() << " + " << imp_one.imag() << "j, " << imp_two.real() << " + " << imp_two.imag() << "j" << endl;
  }
}
