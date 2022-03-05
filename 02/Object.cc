/* 

Rachel Ng 

Object constructors and functions

*/

#include "Object.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

// constructors 
Object::Object(): label_{-1}, pixels_{set<pair<int,int>> {}}, center_area_{-1} { }
Object::Object(int label): label_{label}, pixels_{set<pair<int,int>> {}}, center_area_{-1} { }

// getters 
int Object::Label() const { return label_; }
set<pair<int,int>> Object::Pixels() const { return pixels_; }
int Object::area() const { return pixels_.size(); }

// retrieve center {x,y} or set and return 
pair<int,int> Object::center(){
    if (center_area_ != -1 && area() == center_area_) {
        return center_;
    }
    return moment_1();
}

/*
return orientation in degrees or radians

INPUT   optional bool for conversion to degrees
OUTPUT  orientation 
*/
double Object::orientation(bool degrees) const {
    return !degrees ? orientation_ : orientation_ * 180.0 / M_PI;
}


/*
insert pixel to pixels_ vector

INPUT   pixel {x, y}
*/
void Object::AddPixel(pair<int,int> pixel) {
    pixels_.insert(pixel);
}

// area of object
int Object::moment_0(){
    return pixels_.size();
}

// center of object 
pair<int, int> Object::moment_1(){
    int sum_x = 0;
    int sum_y = 0;

    for (auto p : pixels_) {
        sum_x += p.first;
        sum_y += p.second;
    }

    center_ = {sum_x / area(), sum_y / area()};
    center_area_ = area();
    return center_;
}

/*
orientation of object
updates center if needed
does all calculations for second moment 
set minimum moment of inertia, roundness, and orientation

OUTPUT  minimum moment of inertia, maximum moment of inertia
*/
pair<double, double> Object::moment_2(){
    double a = 0;
    double b = 0; 
    double c = 0; 

    max_x_ = center().first;
    max_y_ = center().second;
    min_x_ = center().first;
    min_y_ = center().second;
    
    // do calculations
    for (auto pixel : pixels_) {
        double x_prime = (pixel.first - center().first);
        double y_prime = (pixel.second - center().second);

        max_x_ = max(max_x_, pixel.first);
        max_y_ = max(max_y_, pixel.second);
        min_x_ = min(min_x_, pixel.first);
        min_y_ = min(min_y_, pixel.second);

        a += pow(x_prime, 2); 
        b += (x_prime * y_prime);
        c += pow(y_prime, 2);
    }
    
    b *= 2;

    const double theta1 = atan2(b, a-c) / 2.0;

    // minimum moment of inertia 
    const double e_min = a * sin(theta1) * sin(theta1) - b * sin(theta1) * cos(theta1) + c * cos(theta1) * cos(theta1);


    const double theta2 = theta1 + M_PI / 2.0;

    // maximum moment of inertia 
    const double e_max = a * sin(theta2) * sin(theta2) - b * sin(theta2) * cos(theta2) + c * cos(theta2) * cos(theta2);


    minimum_moment_inertia_= e_min;
    roundedness_ = e_min / e_max; 
    orientation_ = theta1; 

    return {e_min, e_max};
}

// return database string 
string Object::db() const {
    string s = to_string(label_) + " "; 
    s += to_string(center_.first) + " ";
    s += to_string(center_.second) + " ";
    s += to_string(minimum_moment_inertia_) + " ";
    s += to_string(area()) + " ";
    s += to_string(roundedness_) + " ";
    s += to_string(orientation_ * 180.0 / M_PI) + " ";
    return s;
}
