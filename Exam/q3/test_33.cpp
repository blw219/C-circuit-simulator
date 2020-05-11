#include "network.hpp"

int main()
{
  vector<float> a = make_log10_space(1, 10, 2);
  vector<float> b = make_log10_space(1, 100, 3);
  vector<float> c = make_log10_space(0.01, 100, 3);
  vector<float> d = make_log10_space(1, 10, 5);
  vector<float> e = make_log10_space(0.1, 10, 5);

  for(int i=0; i<a.size(); i++){
    cout << a[i] << ", ";
  }
  cout << endl;

  for(int i=0; i<b.size(); i++){
    cout << b[i] << ", ";
  }
  cout << endl;

  for(int i=0; i<c.size(); i++){
    cout << c[i] << ", ";
  }
  cout << endl;

  for(int i=0; i<d.size(); i++){
    cout << d[i] << ", ";
  }
  cout << endl;

  for(int i=0; i<e.size(); i++){
    cout << e[i] << ", ";
  }
  cout << endl;
}
