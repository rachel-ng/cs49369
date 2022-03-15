#ifndef UTIL
#define UTIL 

/*

operator overloading << 
    modified from 
    https://www.geeksforgeeks.org/operator-overloading-cpp-print-contents-vector-map-pair/

*/

#include<iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>



template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T, S>& p); 

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s);

template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, S>& m); 



template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T, S>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (int i = 0; i < v.size(); i++) {
        os << v[i];
        if (i != (v.size() - 1)) os << ", ";
    }
    os << "]";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    os << "{";
    for (auto i : s) {
        os << i;
        if (i != *s.rbegin()) os << ", ";
    }
    os << "}";
    return os;
}

template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, S>& m) {
    for (auto i : m) {
        os << i.first << "\t" << i.second << "\n"; 
    }
    return os;
}


#endif

