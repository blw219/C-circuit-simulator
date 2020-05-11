#include "network.hpp"

using namespace std;

int main()
{
  Network a = C(0.5);
  Network b = C(1);
  Network c = C(2);
  Network d = R(1e-3);
  Network e = R(1);
  Network f = L(0.3);
  Network g = L(10);

  Network w {'|',0,{C(1), C(2)}};
  Network x {'|',0,{R(1), C(2)}};
  Network y {'&',0,{C(2), L(0.1)}};
  Network z {'&',0,{C(2), R(3)}};
  cout << (w < x) << " " << (y < z) << endl;
}
