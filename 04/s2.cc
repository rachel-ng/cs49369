/* 

Rachel Ng 

s2 - calculate light source directions 

USAGE: ./s2 parameters_file image_1 image_2 image_3 directions_file 

*/

#include "image.h"
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

    for (int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int cur = img->GetPixel(i,j);
            if (std::max(brightest, cur) == cur) {
                brightest = cur;
                brightest_pixel = {i,j};
            }
        }
    }   

    return brightest_pixel;
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

std::vector<double> LightDirection(ComputerVisionProjects::Image *img, std::pair<int, int> center, int radius) {
    std::pair<int, int> bright = GetBrightest(img);
    std::vector<double> bright_vector = CalculateVector(center, radius, bright);
    std::vector<double> bright_norm = NormalVector(bright_vector, img->GetPixel(bright.first, bright.second));
    return bright_norm;
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

    std::vector<double> light_1 = LightDirection(&img_1, center, radius);
    std::vector<double> light_2 = LightDirection(&img_2, center, radius);
    std::vector<double> light_3 = LightDirection(&img_3, center, radius);
    std::vector<std::vector<double>> directions = {light_1, light_2, light_3};

    std::ofstream output_stream(directions_file);
    for (std::vector<double> light : directions) { 
        output_stream << light[0] << " " << light[1] << " " << light[2] << std::endl;
    }
    output_stream.close();
}
