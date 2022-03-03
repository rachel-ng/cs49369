/* 

Rachel Ng 

p3 - 

*/

#include "image.h"
#include "util.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


using namespace std;
using namespace ComputerVisionProjects;



/*
x object label
x row position of the center
x column position of the center
the minimum moment of inertia
x object area
roundedness
orientation (angle in degrees between the axis of minimum inertia and the vertical axis)
*/


class Object {
    public: 
        Object(): label_{-1}, pixels_{set<pair<int,int>> {}}, center_area_{-1} { }
        Object(int label): label_{label}, pixels_{set<pair<int,int>> {}}, center_area_{-1} { }
        

        int Label() const { return label_; }
        set<pair<int,int>> Pixels() const { return pixels_; }
        int area() const { return pixels_.size(); }

        pair<int,int> retrieveCenter() const{
            // cout << area() << "\t" << center_area_ << endl;
            return area() == center_area_ ? center_ : pair<int,int> {-1,-1};
        }

        pair<int,int> center(){
            if (center_area_ != -1 && area() == center_area_) {
                return center_;
            }

            int sum_x = 0;
            int sum_y = 0;

            for (auto p : pixels_) {
                sum_x += p.first;
                sum_y += p.second;
            }

            if (false) cout << label_ << "\tcenter (" << sum_x / area() << ", " << sum_y / area() << ")" << endl;

            center_ = {sum_x / area(), sum_y / area()};
            center_area_ = area();
            return center_; 
        }

        void AddPixel(pair<int,int> pixel) {
            pixels_.insert(pixel);
        }

        string str() const {
            string s = ""; 
            s += to_string(label_) + "\t";
            // s += "[(" + to_string(pixels_.begin()->first) + ", " + to_string(pixels_.begin()->second) + "), ...]\t";  
            s += "center (" + to_string(center_.first) + ", " + to_string(center_.second) + ")\t"; 
            s += "minimum of inertia " + to_string(minimum_moment_inertia_) + "\t";
            s += "area " + to_string(area()) + "\t";
            s += "roundedness " + to_string(roundedness_) + "\t";
            s += "orientation " + to_string(orientation_) + "\t";
            return s;
        }

        string db() const {
            string s = to_string(label_) + " "; 
            s += to_string(center_.first) + " ";
            s += to_string(center_.second) + " ";
            s += to_string(minimum_moment_inertia_) + " ";
            s += to_string(area()) + " ";
            s += to_string(roundedness_) + " ";
            s += to_string(orientation_) + " ";
            return s;
        }

    private: 
        int label_; 
        set<pair<int,int>> pixels_;
        pair<int, int> center_; 
        int center_area_;

        float minimum_moment_inertia_;

        float roundedness_; 

        float orientation_; 

};

ostream& operator<<(ostream& os, const Object object) { 
    os << object.str(); 
    return os;
}

float MinimumMomentOfInertia(Object object) {
    pair<int, int> center = object.center();
    double a = 0;
    double b = 0; 
    double c = 0; 
    
    for (auto pixel : object.Pixels()) {
        double x_prime = (pixel.first - center.first);
        double y_prime = (pixel.second * center.second);
        a += pow(x_prime, 2); 
        b += (x_prime * y_prime) * 2;
        c += pow(y_prime, 2);
    }
    
    const double theta1 = atan2(b, a-c) / 2.0;
    const double theta1_degrees = 180.0 * theta1 / M_PI;

    // minimum moment of inertia 
    const double e_min = a * sin(theta1) * sin(theta1) - b * sin(theta1) * cos(theta1) + c * cos(theta1) * cos(theta1);

    const double theta2 = theta1 + M_PI / 2.0;

    const double e_max = a * sin(theta2) * sin(theta2) - b * sin(theta2) * cos(theta2) + c * cos(theta2) * cos(theta2);
    const double theta2_degrees = 180.0 * theta2 / M_PI;

    return e_min;
}


/*
const double theta1 = atan2(b, a-c) / 2.0; // theta1 in radians
// If you want to see it in degrees.
const double theta_degrees = 180.0 * theta1 / M_PI;
// => Emin
const double e_min = a * sin(theta1) * sin(theta1) – b * sin(theta1)
*cos(theta1) + c * cos(theta1) * cos(theta1);
// Use radians as parameters in cos(), sin(), etc.
// If you have theta1 you can compute rho1. How?
// => You will get the equation of line: (theta1, rho1)
const double theta2 = theta1 + M_PI / 2.0;
// => Emax
const double e_max =
*/


void GenerateDBEntry(Image *img) {
    if (img == nullptr) abort();
    
    const int rows = img->num_rows();
    const int cols = img->num_columns();
    unordered_map<int,Object> objects;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);
            if (pixel_color != 0 && false) cout << "(" << i << ", " << j << ")\t" << pixel_color << endl;
            if (pixel_color != 0) { 
                if (objects.count(pixel_color) == 0) objects[pixel_color] = Object(pixel_color); 
                objects[pixel_color].AddPixel({i,j});
            }
        }
    }

    cout << endl << "ITERATE" << endl;

    for (auto &o : objects) {
        // cout << o << endl;
        pair<int,int> oc = o.second.center();
        img->SetPixel(oc.first, oc.second, 255);
    }

    cout << endl << objects << endl;

}

int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("Usage: %s file1 db_file file2\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string output_database(argv[2]);
    const string output_file(argv[3]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }

    GenerateDBEntry(&img); 
  
    if (false) cout << img.num_rows() << " " << img.num_columns() << "\t" << img.num_gray_levels() << endl;
    if (!WriteImage(output_file, img)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

