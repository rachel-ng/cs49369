/* 

Rachel Ng 

p1 - converts gray-level image to binary using a given threshold 

*/

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;


/*
binarize image using a threshold

INPUT   image to threshold, int to use as threshold 
*/
void ThreshImage (Image *img, int threshold) {
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
        printf("USAGE: %s input_image threshold binary_image\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const int threshold(stoi(argv[2]));
    const string output_file(argv[3]);

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }


    ThreshImage(&an_image, threshold); 
  

    if (!WriteImage(output_file, an_image)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

