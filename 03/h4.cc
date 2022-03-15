/* 

Rachel Ng 

h4 - Hough Transform 
USAGE: ./h4 input_image voting_array threshold output_image

*/

#include "image.h"
#include "Object.h"
#include "imutils.h"
#include "util.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace ComputerVisionProjects;



bool between(int small, int value, int big) {
    return small < value && value < big;
}


/*
compute rho using equation
    rho = (x * std::cos(theta)) + (y * std::sin(theta)) 

INPUT   theta, x, y 
*/
int calculateRho(double theta, int x, int y) {
    return (x * std::cos(theta)) + (y * std::sin(theta)); 
}

/*
compute missing value set to -1 
    rho = (x * std::cos(theta)) + (y * std::sin(theta)) 

INPUT   rho, theta, x, y 
*/
int calculateMissing(int rho, double theta, int x, int y) {
    if (x == -1) {
        return (rho - (y * std::sin(theta))) / std::cos(theta);
    }
    else if (y == -1) {
        return (rho - (x * std::cos(theta))) / std::sin(theta);
    }
}

/*
get voting array from file 

INPUT   voting array file name
        max rho and max theta 
OUTPUT  vector<vector <int>> voting_array[rho][theta]
*/
std::vector<std::vector<int>> GetVotingArray(std::string voting_array_file, int max_rho, int max_theta) {
    std::vector<std::vector<int>> voting_array(max_rho, std::vector<int> (max_theta, 0));

    std::ifstream voting_array_stream(voting_array_file);
    std::string line;
    while(std::getline(voting_array_stream, line)){
        std::stringstream current(line);
        int rho, theta, votes;
        while (current >> rho >> theta >> votes) {
            // std::cout << "voting_array[" << rho << "][" << theta << "] = " << votes << std::endl;
            voting_array[rho][theta] = votes;
        }
    }
    voting_array_stream.close();

    return voting_array;
}

int main(int argc, char **argv){
  
    if (argc!=5) {
        printf("USAGE: %s input_image voting_array threshold output_image\n", argv[0]);
        return 0;
    }

    const std::string input_file(argv[1]);
    const std::string voting_array_file(argv[2]);
    const int threshold(std::stoi(argv[3]));
    const std::string output_file(argv[4]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        std::cout <<"FILE " << input_file << std::endl;
        return 0;
    }


    const int rows = img.num_rows();
    const int cols = img.num_columns(); 
    int max_rho = std::sqrt(std::pow(rows, 2) + std::pow(cols, 2)) + 1;
    int max_theta = 360 + 1;

    std::vector<std::vector<int>> voting_array = GetVotingArray(voting_array_file, max_rho, max_theta);


    // create image of hough space 
    Image hough_space_img;
    HoughSpaceImage(&hough_space_img, voting_array);
    ThresholdImage(&hough_space_img, threshold); 
    std::unordered_map<int, Object> objects = ConnectedComponents(&hough_space_img);


    if (!WriteImage("thresh_" + output_file, hough_space_img)){
        std::cout << "FILE " << "thresh_" + output_file << std::endl;
        return 0;
    }

    std::cout << rows << "x" << cols << std::endl;

    // calculate weighted average center 
    for (auto &obj : objects) {
        std::set<std::pair<int, int>> pixels = obj.second.pixels_;
		std::cout << pixels << std::endl;

        int center_area = 0; 
        int center_row = 0;
        int center_col = 0;

		for (auto pixel : pixels) {
            int votes = hough_space_img.GetPixel(pixel.first, pixel.second);
            center_row += pixel.first * votes;
            center_col += pixel.second * votes; 
            center_area += votes;
		}

        obj.second.center_ = {center_row / center_area, center_col / center_area};
    } 

    for (auto &obj : objects) {
        std::pair<int, int> center = obj.second.center_;
        std::cout << center << std::endl;

        std::pair<int, int> x0 = {0, calculateMissing(center.first, center.second * (M_PI / 180.0), 0, -1)};
        if (!between(-1, x0.second, cols)) {
            x0 = {rows-1, calculateMissing(center.first, center.second * (M_PI / 180.0), rows-1, -1)};
        } 

        std::pair<int, int> y0 = {calculateMissing(center.first, center.second * (M_PI / 180.0), -1, 0), 0};
        if (!between(-1, y0.first, rows)) {
            y0 = {calculateMissing(center.first, center.second * (M_PI / 180.0), -1, cols-1), cols-1};
        }

        if (between(-1, x0.second, cols) && between(-1, y0.first, rows)) { 
            std::cout << x0 << "\t" << y0 << std::endl;

            ComputerVisionProjects::DrawLine(x0.first, x0.second,
                                             y0.first, y0.second,
                                             255, 
                                             &img);
            std::cout << x0 << "\t" << y0 << std::endl;
        }
    }

    if (!WriteImage(output_file, img)){
        std::cout << "FILE " << output_file << std::endl;
        return 0;
    }

}

