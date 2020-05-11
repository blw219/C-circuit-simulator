#include "network.hpp"
#include <map>
#include <algorithm>

bool net_vec_compare(const Network &a, const Network &b) { //parameter for sorting networks (not sure if works for |/&)
    map<char,int> weight;
    weight.insert({'&', 1});
    weight.insert({'C', 2});
    weight.insert({'L', 3});
    weight.insert({'R', 4});
    weight.insert({'|', 5});

    int i=-1;
    if (a.type != b.type) { // if different type, C<L<R
        return weight.find(a.type)->second < weight.find(b.type)->second;
    } else { //if same type, compare value.

        if ((a.type == b.type) && (a.value == 0 && b.value ==0)) { //both series or both parallel
            i++;
            net_vec_compare(a.parts[i], b.parts[i]);
        }

        return a.value < b.value;
    }
}

bool operator<(const Network &a, const Network &b) //TODO:
{ //assuming a valid input is given.
    map<char,int> weight;
    weight.insert({'&', 1});
    weight.insert({'C', 2});
    weight.insert({'L', 3});
    weight.insert({'R', 4});
    weight.insert({'|', 5});

    if ( is_primitive(a) && is_primitive(b) ) { //CASE 1: both primitives
        if (a.type != b.type) { // if different type, C<L<R
            return weight.find(a.type)->second < weight.find(b.type)->second;
        } else { //if same type, compare value.
            return a.value < b.value;
        }
    } else if (b.type == '|' && a.type != '|') { // CASE 2a: b must be larger than a
        return true; 
    } else if (a.type == '|' && b.type != '|') { // CASE 2b: a must be larger than b
        return false;
    }else { //CASE 3: both are parallel or both are series. assume everything inside is primitive.

        vector<Network> a_parts = a.parts;
        vector<Network> b_parts = b.parts;

        sort(a_parts.begin(), a_parts.end(), net_vec_compare);
        sort(b_parts.begin(), b_parts.end(), net_vec_compare);

        return (a.parts[0] < b.parts[0]);
    }

}









/*
bool operator<(const Network &a, const Network &b) //TODO:
{ //assuming a valid input is given.

    map<char,int> weight;
    weight.insert({'&', 1});
    weight.insert({'C', 2});
    weight.insert({'L', 3});
    weight.insert({'R', 4});
    weight.insert({'|', 5});

    if (a.type == b.type) {
        if (a.value == b.value) {
            if (a.parts.size() == b.parts.size()) {

                if (a.parts.size()==0) { //b.parts.size has to be zero as well, then equal.
                    return false;
                }

                for (int i=0; i<a.parts.size(); i++) {
                    for (int j=0; j<b.parts.size(); j++) {
                        a.parts[i]<b.parts[j];
                    }   
                }

            } else {
                return a.parts.size()<b.parts.size();
            }
        } else {
            return a.value < b.value;
        }
    } else {
        return weight.find(a.type)->second < weight.find(b.type)->second;
    }

}
*/