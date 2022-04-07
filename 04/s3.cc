/* 

Rachel Ng 

s3 - 

USAGE: ./s3 input_directions image_1 image_2 image_3 step threshold output


*/

#include "image.h"
#include "util.h"

#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

/*

0 1 2   a b c 
3 4 5   d e f 
6 7 8   g h i 

0a 1b 2c 
3d 4e 5f 
6g 7h i8

0 1 2   11 12 13
3 4 5   21 22 23
6 7 8   31 32 33

0 11  1 12  2 13 
3 21  4 22  5 23 
6 31  7 32  8 33 

*/

double det(double a, double b, double c, double d) { 
    return (a*d) - (c*b);
}

double det(std::vector<double> v) {
    return (v[0]*det(v[4],v[5],v[7],v[8])) - (v[1]*det(v[3],v[5],v[6],v[8])) + (v[2]*det(v[3],v[4],v[6],v[7]));
}

std::vector<double> adjacent(std::vector<double> v) {
    return std::vector<double> {
        det(v[4], v[5], v[7], v[8]), -det(v[1], v[2], v[7], v[8]), det(v[1], v[2], v[4], v[5]), 
        -det(v[3], v[5], v[6], v[8]), det(v[0], v[2], v[6], v[8]), -det(v[0], v[2], v[3], v[5]), 
        det(v[3], v[4], v[6], v[7]), -det(v[0], v[1], v[6], v[7]), det(v[0], v[1], v[3], v[4])
    }; 
}

std::vector<double> transpose(std::vector<double> v, int r=3, int c=3) {
    std::vector<double> t;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            t.push_back(v[(j*3)+i]);
        }
    }
    return t;
}

std::vector<double> inverse(std::vector<double> v) {
    std::vector<double> adj = adjacent(v);
    double determinant = det(v);

    std::vector<double> inv; 
    for (double i : adj) {
        inv.push_back(i / determinant);
    }
    return inv;
}

/*
double det(std::vector<double> v) {
    double a = v[0]; 11 
    double b = v[1]; 12
    double c = v[2]; 13 
    double d = v[3]; 21
    double e = v[4]; 22 
    double f = v[5]; 23
    double g = v[6]; 31 
    double h = v[7]; 32 
    double i = v[8]; 33 

    return (v[0]*det(v[4],v[5],v[7],v[8])) - (v[1]*det(v[3],v[5],v[6],v[8])) + (v[2]*det(d,e,g,h));
}
*/

std::vector<double> GetDirections(std::string file_name) {  
    std::ifstream file_stream(file_name);
    std::vector<double> directions; 
    double direction;

    while (file_stream >> direction) {
        directions.push_back(direction);
    }

    return directions;
}

int main(int argc, char **argv){

    if (argc!=8) {
        printf("USAGE: %s input_directions image_1 image_2 image_3 step threshold output\n", argv[0]);
        return 0;
    }

    const std::string input_directions(argv[1]); 
    const std::string image_1(argv[2]);
    const std::string image_2(argv[3]);
    const std::string image_3(argv[4]);
    const std::string step(argv[5]);
    const std::string threshold(argv[6]);
    const std::string output(argv[7]);
    
    std::vector<double> directions = GetDirections(input_directions);
    std::cout << directions << std::endl;

    double v_det = det(directions); 
    std::cout << "det\t" << v_det << std::endl;

    std::vector<double> adj = adjacent(directions);
    std::cout << "adj\t" <<  adj << std::endl;

    std::vector<double> trans= transpose(adj);
    std::cout << "trans\t" << trans << std::endl;
    
    std::vector<double> inv = inverse(directions);
    std::cout << "inv\t" << inv << std::endl;
    
    std::cout << "inv inv\t" << inverse(inv) << std::endl;
}


