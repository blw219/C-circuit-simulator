#ifndef network_hpp
#define network_hpp

#include <vector>
#include <iostream>
#include <complex>
#include <cassert>

using namespace std;

struct Network
{
    char type;
    float value;
    vector<Network> parts;
};

/* Create primitive components of the given type. */
Network R(float r);
Network L(float r);
Network C(float r);

/* Create parallel or serial composition of other networks. */
Network operator|(const Network &a, const Network &b);
Network operator&(const Network &a, const Network &b);

/* Return true if the network is a resistor, capacitor or inductor. */
bool is_primitive(const Network &a);

/* Return true if the network is a parallel of series composition. */
bool is_composite(const Network &a);

bool operator==(const Network &a, const Network &b);
bool operator<(const Network &a, const Network &b);


istream &operator>>(istream &src, Network &c);
ostream &operator<<(ostream &dst, const Network &c);

/* Return the impedance of the network at given frequency omega. */
complex<float> impedance(const Network &c, float omega);

/*  Given a factor of frequencies and two networks, return the transfer function
    for n1 and n2 as a potential divider at the given frequencies.
    H = Z(n2,omega) / (Z(n1,omega)+Z(n2,omega))
*/
vector<complex<float>> transfer_function(const Network &n1, const Network &n2, const vector<float> &omega);

/* Return a vector of values in the inclusive range [xLo,xHi], containing
    n logarithmically spaced points. */
vector<float> make_log10_space(float xLo, float xHi, int n);

/* Creates a variety of networks which can be useful for testing.
*/
vector<Network> create_test_networks();

/* Return a network that is equivalent to x, but in the
    standardised canonical form.
*/
Network canonicalise(const Network &x);

/* Overload less than operator
*/
vector<Network> sort_terms(vector<Network> parts);

bool is_sorted(vector<Network> a);

void flatten(char type, vector<Network> &a);



#endif
