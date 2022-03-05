/* 

Rachel Ng 

p3 - 

*/

#include "image.h"
#include "util.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
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
        int max_x_;
        int max_y_;
        int min_x_;
        int min_y_; 

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
            return moment_1();
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
            s += "orientation " + to_string(orientation_degrees_) + "\t";
            return s;
        }

        string db() const {
            string s = to_string(label_) + " "; 
            s += to_string(center_.first) + " ";
            s += to_string(center_.second) + " ";
            s += to_string(minimum_moment_inertia_) + " ";
            s += to_string(area()) + " ";
            s += to_string(roundedness_) + " ";
            s += to_string(orientation_degrees_) + " ";
            return s;
        }

        int moment_0(){
            // area 
            return pixels_.size();
        }

        pair<int, int> moment_1(){
            // center of area 
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

        pair<double, double> moment_2(){
            bool debug = false;
            double a = 0;
            double b = 0; 
            double c = 0; 

            max_x_ = center().first;
            max_y_ = center().second;
            min_x_ = center().first;
            min_y_ = center().second;
            
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
            const double theta1_degrees = 180.0 * theta1 / M_PI;

            if (debug) cout << "THETA1\t" << theta1 << "\t" << theta1_degrees << endl;

            // minimum moment of inertia 
            const double e_min = a * sin(theta1) * sin(theta1) - b * sin(theta1) * cos(theta1) + c * cos(theta1) * cos(theta1);

            const double theta2 = theta1 + M_PI / 2.0;
            const double theta2_degrees = 180.0 * theta2 / M_PI;

            if (debug) cout << "THETA2\t" << theta2 << "\t" << theta2_degrees << endl;

            // maximum moment of inertia 
            const double e_max = a * sin(theta2) * sin(theta2) - b * sin(theta2) * cos(theta2) + c * cos(theta2) * cos(theta2);


            e_min_ = e_min;
            e_max_ = e_max;
            minimum_moment_inertia_= e_min_;
            roundedness_ = e_min_ / e_max_; 
            orientation_ = theta1; 
            orientation_degrees_ = theta1_degrees; 

            if (debug) cout << e_min << "\t" << e_max << endl;

            return {e_min, e_max};
        }

        double orientation() {
            return orientation_;
        }


    private: 
        int label_; 
        set<pair<int,int>> pixels_;
        pair<int, int> center_; 
        int center_area_;

        double e_min_; 
        double e_max_; 
        double minimum_moment_inertia_;

        double roundedness_; 

        double orientation_; 
        double orientation_degrees_; 

};

ostream& operator<<(ostream& os, const Object object) { 
    os << object.str(); 
    return os;
}

unordered_map<int,Object> GenerateDBEntry(Image *img) {
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

    for (auto &o : objects) {
        // cout << o << endl;
        pair<int,int> oc = o.second.center();
        o.second.moment_2();
        img->SetPixel(oc.first, oc.second, 255);
        // cout << o.second.min_x_ << "\t" << o.second.min_y_ << "\t" << o.second.max_x_ << "\t" << o.second.max_y_ << endl;

        
        DrawLine(oc.first, 
                oc.second, 
                round((o.second.max_x_ - oc.first) * cos(o.second.orientation()) + oc.first), 
                round((o.second.max_x_ - oc.first)* sin(o.second.orientation()) + oc.second), 
                255, 
                img);

        cout << o.second.db() << endl;
    }

    cout << endl << objects << endl;

    return objects;
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

    cout << input_file << endl;


    unordered_map<int,Object> objects_map = GenerateDBEntry(&img); 

    ofstream database_file(output_database);
    for (auto &o : objects_map) {
        database_file << o.second.db() << endl;
    }
    database_file.close();

  
    if (false) cout << img.num_rows() << " " << img.num_columns() << "\t" << img.num_gray_levels() << endl;
    if (!WriteImage(output_file, img)){
        cout << "FILE " << output_file << endl;
        return 0;
    }

}

