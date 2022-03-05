#ifndef OBJECT_H_
#define OBJECT_H_

/* 

Rachel Ng 

Object header file 

*/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

class Object{
    public:
        int max_x_;
        int max_y_;
        int min_x_;
        int min_y_;

        Object();
        Object(int label);

        int Label() const;
        std::set<std::pair<int,int> > Pixels() const;
        int area() const;
        pair<int,int> center();
        double orientation(bool degrees=false) const;

        void AddPixel(pair<int,int> pixel);

        int moment_0();
        pair<int, int> moment_1();
        pair<double, double> moment_2();

        string str() const;
        string db() const;


    private: 
        int label_; 
        set<pair<int,int>> pixels_;
        pair<int, int> center_; 
        int center_area_;

        double minimum_moment_inertia_;

        double roundedness_; 

        double orientation_; 
};

#endif

