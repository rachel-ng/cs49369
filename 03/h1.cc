/* 

Rachel Ng 

h1 - find locations of edge points in image 
USAGE: ./h1 input_image output_image 

*/

#include "image.h"
#include "Pixel.h"
#include "util.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace ComputerVisionProjects;


std::vector<std::vector<int>> sobel_x = {{-1,  0,  1}, 
                                         {-2,  0,  2}, 
                                         {-1,  0,  1}};

std::vector<std::vector<int>> sobel_y = {{ 1,  2,  1}, 
                                         { 0,  0,  0}, 
                                         {-1, -2, -1}};


bool between(int small, int value, int big) {
    return small < value && value < big;
}


/*
apply a convolution to a pixel 

INPUT   Image img to get values 
        vector<vector<int>> kernel to apply 
        Pixel point 
OUTPUT  sum of values 
*/
int applyConvolution(Image *img, std::vector<std::vector<int>> kernel, Pixel point) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 
    int sum = 0;

    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            Pixel pixel {point.x + r, point.y + c};
            if (between(-1, pixel.x, rows) && between(-1, pixel.y, cols)) { 
                sum += img->GetPixel(pixel.x, pixel.y) * kernel[1+r][1+c];
            }
        }
    }

    return sum;
}

/*
find locations of edges by applying Sobel's convolution masks 
write new values to image 

INPUT Image img 
*/
void locateEdges(Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 
    std::vector<std::vector<int>> gradient_values(rows, std::vector<int> (cols));
    
    // calculate gradient values 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int gradient_x = applyConvolution(img, sobel_x, {i,j}); 
            int gradient_y = applyConvolution(img, sobel_y, {i,j}); 
            int gradient_value = sqrt(pow(gradient_x,2) + pow(gradient_y,2));
            gradient_values[i][j] = (i == 0 || j == 0) ? 0 : gradient_value;
            // gradient_values[i][j] = gradient_value;
        }
    }

    // write values to image 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            img->SetPixel(i, j, gradient_values[i][j]);
        }
    }
}


int main(int argc, char **argv){
  
    if (argc!=3) {
        printf("USAGE: %s input_image output_image\n", argv[0]);
        return 0;
    }

    const std::string input_file(argv[1]);
    const std::string output_file(argv[2]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        std::cout <<"FILE " << input_file << std::endl;
        return 0;
    }


    locateEdges(&img);


    if (!WriteImage(output_file, img)){
        std::cout << "FILE " << output_file << std::endl;
        return 0;
    }

}

