/* 

Rachel Ng 

s1 - calculate center coordinates and radius of sphere 

USAGE: ./s1 input_image threshold output_file  

*/

#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

/*
binarize image using a threshold
INPUT   image to threshold, int to use as threshold 
*/
void ThresholdImage(ComputerVisionProjects::Image *img, int threshold) {
    if (img == nullptr) abort();
    
    const int rows = img->num_rows();
    const int cols = img->num_columns();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            img->SetPixel(i,j, img->GetPixel(i,j) < threshold ? 0 : 255);
        }
    }

    img->SetNumberGrayLevels(1);
}

int main(int argc, char **argv){

    if (argc!=4) {
        printf("USAGE: %s input_image threshold output_file\n", argv[0]);
        return 0;
    }

    const std::string input_file(argv[1]);
    const int threshold(std::stoi(argv[2]));
    const std::string output_file(argv[3]);

    ComputerVisionProjects::Image img;

    if (!ReadImage(input_file, &img)) {
        std::cout << "FILE " << input_file << "\tunable to read" << std::endl;
        return 0;
    }

    const int rows = img.num_rows();
    const int cols = img.num_columns(); 

    ThresholdImage(&img, threshold); 

    int sum_x = 0;
    int sum_y = 0;
    int area = 0;
    std::pair<int, int> l_most = {0, rows};
    std::pair<int, int> r_most = {0, -1};
    std::pair<int, int> u_most = {cols + 1, 0};
    std::pair<int, int> b_most= {-1, 0};
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (img.GetPixel(i,j) == 255) {
                std::pair <int,int> pixel = {i,j};
                l_most = std::min(l_most.second, j) == l_most.second ? l_most : pixel;
                r_most = std::max(r_most.second, j) == r_most.second ? r_most : pixel;
                u_most = std::min(u_most.first, i) == u_most.first ? u_most : pixel;
                b_most = std::max(b_most.first, i) == b_most.first ? b_most : pixel;
                sum_x += i;
                sum_y += j;
                area++;
            }
        }
    }

    std::pair<int,int> center = {sum_x/area, sum_y/area};
    std::cout << "center: " << center << std::endl;
    std::cout << "lmost: " << l_most << std::endl;
    std::cout << "rmost: " << r_most << std::endl;
    std::cout << "umost: " << u_most << std::endl;
    std::cout << "bmost: " << b_most << std::endl;

    double lm_dist = std::sqrt(std::pow(l_most.first - r_most.first, 2) + std::pow(l_most.second - r_most.second, 2)); 
    double ub_dist = std::sqrt(std::pow(u_most.first - b_most.first, 2) + std::pow(u_most.second - b_most.second, 2)); 
    
    std::cout << "lm " << lm_dist << std::endl;
    std::cout << "ub " << ub_dist << std::endl;
    std::cout << "avg " << std::round((lm_dist + ub_dist)/4) << std::endl;
    
    
    std::ofstream output_stream(output_file);
    output_stream << center.first << " " << center.second << " " << std::round((lm_dist + ub_dist)/4);
    output_stream.close();
}

