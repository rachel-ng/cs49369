/* 

Rachel Ng 

s4 - albedo 

USAGE: ./s4 input_directions image_1 image_2 image_3 threshold output


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

std::vector<double> GetDirections(std::string file_name) {  
    std::ifstream file_stream(file_name);
    std::vector<double> directions; 
    double direction;

    while (file_stream >> direction) {
        directions.push_back(direction);
    }

    return directions;
}

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

std::vector<double> CalculateVector(std::pair<int, int> center, int radius, std::pair<int, int> pixel) {
    double x = pixel.first - center.first;
    double y = pixel.second - center.second;
    double z = std::sqrt(std::pow(radius, 2) - std::pow(x, 2) - std::pow(y, 2));
    double magnitude = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));

    return std::vector<double> {x, y, z, magnitude};
}

std::vector<double> NormalVector(std::vector<double> vec, int brightness) {
    return std::vector<double> {(brightness * vec[0])/vec[3], (brightness * vec[1])/vec[3], (brightness * vec[2])/vec[3]};
}

std::vector<double> GetNormal(ComputerVisionProjects::Image *img, std::pair<int, int> center, int radius, std::pair<int, int> pixel) {
    std::vector<double> vec = CalculateVector(center, radius, pixel);
    int brightness = img->GetPixel(pixel.first, pixel.second);
    return NormalVector(vec, brightness); 
}

bool isVisible(ComputerVisionProjects::Image *img, std::pair<int, int> pixel, int threshold) {
    return img->GetPixel(pixel.first, pixel.second) > threshold;
}

std::vector<double> PixelNorm(std::vector<double> directions, std::vector<int> brightness) {
    std::vector<double> norm;    
    for(int i = 0; i < directions.size(); i += brightness.size()){
        double n = 0;
        for (int j = 0; j < brightness.size(); j++) {
            n += (directions[i+j] * brightness[j]);
        }
        norm.push_back(n);
    }
    return norm;
}

int main(int argc, char **argv){

    if (argc!=7) {
        printf("USAGE: %s input_directions image_1 image_2 image_3 threshold output\n", argv[0]);
        return 0;
    }

    const std::string input_directions(argv[1]); 
    const std::string image_1(argv[2]);
    const std::string image_2(argv[3]);
    const std::string image_3(argv[4]);
    const int threshold(std::stoi(argv[5]));
    const std::string output(argv[6]);
    

    std::vector<double> directions = GetDirections(input_directions);
    const std::vector<double> inv = inverse(directions);

    ComputerVisionProjects::Image img_1;
    if (!ReadImage(image_1, &img_1)) {
        std::cout << "FILE " << image_1 << "\tunable to read" << std::endl;
        return 0;
    }

    ComputerVisionProjects::Image img_2;
    if (!ReadImage(image_2, &img_2)) {
        std::cout << "FILE " << image_2 << "\tunable to read" << std::endl;
        return 0;
    }

    ComputerVisionProjects::Image img_3;
    if (!ReadImage(image_3, &img_3)) {
        std::cout << "FILE " << image_3 << "\tunable to read" << std::endl;
        return 0;
    }

    const int rows = img_1.num_rows();
    const int cols = img_1.num_columns(); 

    std::vector<std::vector<double>> magnitudes(rows, std::vector<double>(cols, 0));
    double max_bright = 0; 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            if (isVisible(&img_1,{i,j},threshold) && isVisible(&img_2,{i,j},threshold) && isVisible(&img_3,{i,j},threshold)) {
                std::vector<double> norm = PixelNorm(inv, std::vector<int> {img_1.GetPixel(i,j), img_2.GetPixel(i,j), img_3.GetPixel(i,j)});
                double magnitude = std::sqrt(std::pow(norm[0],2) + std::pow(norm[1],2) + std::pow(norm[2],2));
                magnitudes[i][j] = magnitude; 
                max_bright = std::max(max_bright, magnitude);
            }
            else {
                img_1.SetPixel(i,j, 0);
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            img_1.SetPixel(i, j, ((255 * magnitudes[i][j]) / max_bright));
        }
    }

    if (!WriteImage(output, img_1)){
        std::cout << "FILE " << output << std::endl;
        return 0;
    }


}


