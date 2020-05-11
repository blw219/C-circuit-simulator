#include "network.hpp"
#include <map>
#include <algorithm>

Network R(float v)
{
    return {'R', v, {} };
}

Network C(float v)
{
    return {'C', v, {} };
}

Network L(float v)
{
    return {'L', v, {} };
}


bool operator==(const Network &a, const Network &b)
{
    if(a.type != b.type) return false;
    if(a.value != b.value) return false;
    return a.parts == b.parts;
}

bool net_vec_compare(const Network &a, const Network &b) { // abandoned parameter for sorting primitive networks (not sure if works for |/&)
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

    if( is_primitive(a) && is_primitive(b) ) { //CASE 1: both primitives
        if (a.type != b.type) { // if different type, C<L<R
          return weight.find(a.type)->second < weight.find(b.type)->second;
        } else { //if same type, compare value.
          return a.value < b.value;
        }
    }else if(a.type == '&' && is_primitive(b)){ // CASE 2a: any primitive must be larger than series
      return true;
    }else if(b.type == '&' && is_primitive(a)){ // CASE 2b: any primitive must be larger than series (opposite of 2b)
      return false;
    }else if(a.type == '|' && is_primitive(b)){ // CASE 3a: any parallel must be larger than primitives
      return false;
    }else if(b.type == '|' && is_primitive(a)){ // CASE 3b: any parallel must be larger than primitives (opposite of 3a)
      return true;
    }else if(a.type == '&' && b.type == '|'){ // CASE 4a: any parallel must be larger than series
      return true;
    }else if(b.type == '&' && a.type == '|'){ // CASE 4b: any parallel must be larger than series (opposite of 4a)
      return false;
    }else if(a.type == '&' && b.type == '&'){ // CASE 5a: both series
      if(a.parts.size() > b.parts.size()){
        for(int i=0; i<b.parts.size(); i++){
          if(a.parts[i] < b.parts[i]){
            return true;
          }
        }
        return false;
      }else if(a.parts.size() <= b.parts.size()){
        for(int i=0; i<a.parts.size(); i++){
          if(a.parts[i] < b.parts[i]){
            return true;
          }
        }
        return false;
      }
    }else if(a.type == '|' && b.type == '|'){ // CASE 5b: both parallel (code same as 5a)
      if(a.parts.size() > b.parts.size()){
        for(int i=0; i<b.parts.size(); i++){
          if(a.parts[i] < b.parts[i]){
            return true;
          }
        }
        return false;
      }else if(a.parts.size() <= b.parts.size()){
        for(int i=0; i<a.parts.size(); i++){
          if(a.parts[i] < b.parts[i]){
            return true;
          }
        }
        return false;
      }
    }

}

Network operator|(const Network &a, const Network &b)
{
    return Network{'|', 0, {a,b} };
}

Network operator&(const Network &a, const Network &b)
{
    return Network{'&', 0, {a,b} };
}

bool is_primitive(const Network &a) //TODO:
{
    if (a.type != 'R' && a.type !='L' && a.type != 'C') {
        return false;
    }

    if (a.value < 0 || a.value == 0) { //non primitive networks have value of 0.
        return false;
    }

    if (!a.parts.empty()) {
        return false;
    }

    return true; //Only return true for resistors, capacitors or inductors with positive values.
}

bool is_composite(const Network &a)
{
    return !is_primitive(a);
}

void flatten(char type, vector<Network> &a)
{   
    for (int i=0; i< a.size(); i++){
      if (type == a[i].type) {
        Network tmp = a[i];
        a.erase(a.begin()+i);
        for(int j=0; j<tmp.parts.size(); j++)
          a.insert(a.begin()+i, tmp.parts[j]);
        }
    }  
}

bool is_sorted(vector<Network> a)
{
    for(int i=0; i<a.size()-1; i++){
      if(!(a[i]<a[i+1])){
        return false;
      }
    }
    return true;
}

vector<Network> sort_terms(vector<Network> parts)
{
  if(is_sorted(parts)){
    return parts;
  }else{
    Network tmp;
    for(int i=0; i<parts.size()-1; i++){
      if(!(parts[i] < parts[i+1])){ //if next part larger than current part, exchange parts
        tmp = parts[i+1];
        parts[i+1] = parts[i];
        parts[i] = tmp;
      }
    }
    return sort_terms(parts);
  }
}


Network canonicalise(const Network &x) //TODO:
{
    if (is_primitive(x)) { //do not attempt to canonicalise primitive networks.
        return x;
    }

    vector<Network> parts = x.parts;

    for (int i=0; i<x.parts.size(); i++) {
        parts[i]= canonicalise(parts[i]);
    }

    flatten(x.type, parts);
    parts = sort_terms(parts);

    Network y;
    y.type = x.type;
    y.parts = parts;
    y.value = 0;

    return y;
}
