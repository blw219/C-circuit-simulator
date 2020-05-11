#include "network.hpp"

int main()
{
  //vector<Network> a = {L(10), L(0.3), R(1), R(1e-3), C(2), C(1), C(0.5)};
  //vector<Network> b = {C(0.5), C(1), C(2), L(0.3), L(10), R(1e-3), R(1), };
  //vector<Network> c = {R(1), R(2)};
  //Network x = {'|', 0, {C(2), L(3)}};
  //Network d = {'|', 0, {R(1), x}};
  Network a = {'&',0,{R(1.3), C(1.2), L(2), L(0.5)}}; //line one of example table in instructions
  Network b = {'&',0,{R(4), R(2), {'|',0,{C(3), C(1)}}, R(1)}}; //line one of example table in instructions
  //a = sort_terms(a);

  //cout << is_sorted(a) << endl;
  a = canonicalise(a);
  b = canonicalise(b);
  cout << a << endl;
  cout << b << endl;
}
