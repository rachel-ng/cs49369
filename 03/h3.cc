/* 

Rachel Ng 

h3 - generate image of Hough Transform of space and voting array
USAGE: ./h3 input_image output_image voting_array 

*/

#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace ComputerVisionProjects;


/*
check if a value is within a range

INPUT   ints small, value, big 
        small < value < big 
OUTPUT  whether value is in range
*/
bool between(int small, int value, int big) {
    return small < value && value < big;
}


/*
solve equation 
    rho = (x * std::cos(theta)) + (y * std::sin(theta)) 
for missing value

INPUT   rho, theta, x, y 
        -1 if you don't have the value
OUTPUT  calculated rho 
*/
int calculateRho(double theta, int x, int y) {
    return (x * std::cos(theta)) + (y * std::sin(theta)); 
}

/*
generate voting array of Hough Transform of space 

INPUT   Image img 
OUTPUT  vector<vector int> voting array
*/
std::vector<std::vector<int>> HoughTransform(Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 
    int max_rho = std::sqrt(std::pow(rows, 2) + std::pow(cols, 2)) + 1;
    int max_theta = 360;
    std::vector<std::vector<int>> voting_array(max_rho, std::vector<int> (max_theta, 0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (img->GetPixel(i, j) == 255) {
                for (int theta = 0; theta < max_theta; theta++){ 
                    int rho = calculateRho(theta * (M_PI / 360.0), i, j);
                    if (between(-1, rho, max_rho)) {
                        voting_array[rho][theta]++;
                    }
                }
            }
        }
    }

    return voting_array;
}


int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("USAGE: %s input_image output_image voting_array\n", argv[0]);
        return 0;
    }

    const std::string input_file(argv[1]);
    const std::string output_file(argv[2]);
    const std::string voting_array_file(argv[3]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        std::cout <<"FILE " << input_file << std::endl;
        return 0;
    }


    std::vector<std::vector<int>> voting_array = HoughTransform(&img);

    Image hough_space_img;
    hough_space_img.AllocateSpaceAndSetSize(voting_array.size(), voting_array[0].size());
    hough_space_img.SetNumberGrayLevels(255);

    std::ofstream voting_array_stream(voting_array_file);
    for (int i = 0; i < voting_array.size(); i++) {
        for (int j = 0; j < voting_array[0].size(); j++){ 
            hough_space_img.SetPixel(i, j, voting_array[i][j]);
            voting_array_stream << voting_array[i][j] << std::endl;
        }
    }
    voting_array_stream.close();


    if (!WriteImage(output_file, hough_space_img)){
        std::cout << "FILE " << output_file << std::endl;
        return 0;
    }

}

