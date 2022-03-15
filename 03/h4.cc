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

    Image hough_space_img;
    hough_space_img.AllocateSpaceAndSetSize(voting_array.size(), voting_array[0].size());
    hough_space_img.SetNumberGrayLevels(255);

    for (int i = 0; i < voting_array.size(); i++) {
        for (int j = 0; j < voting_array[0].size(); j++){ 
            hough_space_img.SetPixel(i, j, voting_array[i][j]);
        }
    }

    ThresholdImage(&hough_space_img, threshold); 
    std::unordered_map<int, Object> objects = ConnectedComponents(&hough_space_img);

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

    
    std::cout << objects << std::endl;

    if (!WriteImage("thresh_" + output_file, hough_space_img)){
        std::cout << "FILE " << "thresh_" + output_file << std::endl;
        return 0;
    }



    if (!WriteImage(output_file, img)){
        std::cout << "FILE " << output_file << std::endl;
        return 0;
    }

}

