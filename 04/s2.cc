/* 

Rachel Ng 

s2 - calculate light source directions 

USAGE: ./s2 parameters_file image_1 image_2 image_3 directions_file 

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


std::vector<int> GetParameters(std::string file_name) {  
    std::ifstream file_stream(file_name);
    std::vector<int> parameters; 
    int param;

    while (file_stream >> param) {
        parameters.push_back(param);
    }

    return parameters;
}

std::pair<int, int> GetBrightest(ComputerVisionProjects::Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 

    std::pair<int,int> brightest_pixel = {-1,-1};
    int brightest = -1;
    // std::cout << "(" << brightest_pixel.first << ", " << brightest_pixel.second << ") " << brightest<< std::endl;

    for (int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int cur = img->GetPixel(i,j);
            if (std::max(brightest, cur) == cur) {
                brightest = cur;
                brightest_pixel = {i,j};
                // ;std::cout << "(" << i << ", " << j << ") " << cur << std::endl;
            }
        }
    }   

    return brightest_pixel;
}


std::vector<int> GetVector(std::pair<int, int> center, int radius, std::pair<int, int> pixel) {
    int x = pixel.first - center.first;
    int y = pixel.second - center.second;
    int z = std::sqrt(std::pow(radius, 2) - std::pow(x, 2) - std::pow(y, 2));
    int magnitude = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));

    return std::vector<int> {x, y, z, magnitude};
}

std::vector<int> NormalVector(std::vector<int> vec, int brightness) {
    return std::vector<int> {(brightness * vec[0])/vec[3], (brightness * vec[1])/vec[3], (brightness * vec[2])/vec[3]};
}

int main(int argc, char **argv){

    if (argc!=6) {
        printf("USAGE: %s parameters_file image_1 image_2 image_3 directions_file\n", argv[0]);
        return 0;
    }

    const std::string parameters_file(argv[1]); 
    const std::string image_1(argv[2]);
    const std::string image_2(argv[3]);
    const std::string image_3(argv[4]);
    const std::string directions_file(argv[5]);

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

    std::vector<int> parameters = GetParameters(parameters_file);
    std::pair<int, int> center = {parameters[0], parameters[1]};
    int radius = parameters[2];
    std::cout << center << "\t" << radius << std::endl << std::endl;

    std::cout << "image 1" << std::endl;
    std::pair<int, int> bright_1 = GetBrightest(&img_1); 
    std::cout << "(" << bright_1.first << ", " << bright_1.second << ")" << std::endl; 
    std::vector<int> vec1 = GetVector(center, radius, bright_1);
    std::vector<int> norm1 = NormalVector(vec1, img_1.GetPixel(bright_1.first, bright_1.second));
    std::cout << "vec\t" << vec1 << std::endl;
    std::cout << "norm\t" << norm1 << std::endl;
    std::cout << std::endl;
    
    std::cout << "image 2" << std::endl;
    std::pair<int, int> bright_2 = GetBrightest(&img_2); 
    std::cout << "(" << bright_2.first << ", " << bright_2.second << ")" << std::endl; 
    std::vector<int> vec2 = GetVector(center, radius, bright_2);
    std::vector<int> norm2 = NormalVector(vec2, img_2.GetPixel(bright_2.first, bright_2.second));
    std::cout << "vec\t" << vec2 << std::endl;
    std::cout << "norm\t" << norm2 << std::endl;
    std::cout << std::endl;
    
    std::cout << "image 3" << std::endl;
    std::pair<int, int> bright_3 = GetBrightest(&img_3); 
    std::cout << "(" << bright_3.first << ", " << bright_3.second << ")" << std::endl; 
    std::vector<int> vec3 = GetVector(center, radius, bright_3);
    std::vector<int> norm3 = NormalVector(vec3, img_3.GetPixel(bright_3.first, bright_3.second));
    std::cout << "vec\t"  << vec3 << std::endl;
    std::cout << "norm\t" << norm3 << std::endl;
    std::cout << std::endl;

    std::vector<std::vector<int>> norms = {norm1, norm2, norm3};

    std::ofstream output_stream(directions_file);
    for (std::vector<int> norm : norms) { 
        output_stream << norm[0] << " " << norm[1] << " " << norm[2] << std::endl;
        std::cout << norm[0] << " " << norm[1] << " " << norm[2] << std::endl;
    }
    output_stream.close();
}

