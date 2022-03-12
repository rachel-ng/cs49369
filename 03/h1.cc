/* 

Rachel Ng 

USAGE: ./h1 input_image output_image 

*/

#include "image.h"
#include "util.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace ComputerVisionProjects;


std::vector<std::vector<int>> sobel_x = {{-1, 0, 1}, 
                                         {-2, 0, 2}, 
                                         {-1, 0, 1}};

std::vector<std::vector<int>> sobel_y = {{ 1,  2,  1}, 
                                         { 0,  0,  0}, 
                                         {-1, -2, -1}};

struct Pixel {
    int x;
    int y; 
    Pixel(int x_, int y_) : x(x_), y(y_) {};
};

bool between(int small, int value, int big) {
    return small < value && value < big;
}

int applyConvolution(Image *img, std::vector<std::vector<int>> filter, Pixel  point) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 

    std::vector<int> values;
    int value = 0;

    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            Pixel pixel {point.x + r, point.y + c};
            // if ((-1 < pixel.x && pixel.x < rows) && (-1 < pixel.y && pixel.y < cols)) {
            if (between(-1, pixel.x, rows) && between(-1, pixel.y, cols)) { 
                int pixel_val = img->GetPixel(pixel.x, pixel.y) * filter[1+r][1+c];
                values.push_back(pixel_val);
                value += pixel_val;
            }
        }
    }

    std::cout << values << std::endl;
    std::cout << value << std::endl;
    return value;
}


void locateEdges(Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns(); 
    std::vector<std::vector<int>> gradient_values(rows, std::vector<int> (cols));
    Image filtered_img = Image();
    filtered_img.AllocateSpaceAndSetSize(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            std::cout << "(" << i << ", " << j << ")\t" << img->GetPixel(i,j) << std::endl;;
            std::cout << "sobel x";
            int gradient_x = applyConvolution(img, sobel_x, Pixel(i,j)); 
            std::cout << "sobel y";
            int gradient_y = applyConvolution(img, sobel_y, Pixel(i,j)); 

            int gradient_value = sqrt(pow(gradient_x,2) + pow(gradient_y,2));
            std::cout << gradient_value << std::endl;
            filtered_img.SetPixel(i, j, gradient_value);


            std::cout << std::endl;
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

    /*
    if (!WriteImage(output_file, img)){
        std::cout << "FILE " << output_file << std::endl;
        return 0;
    }
    */

}

