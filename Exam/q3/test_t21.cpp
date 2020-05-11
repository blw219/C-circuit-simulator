#include "network.hpp"

int main()
{
  Network rc = {'&',0,{R(1),C(1)}};
  Network lc = {'&',0,{L(1),C(1)}};
  Network rclc = {'|',0,{rc, lc}};

  cout << rclc << endl;
}
