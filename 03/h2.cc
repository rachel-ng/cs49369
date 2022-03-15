/* 

Rachel Ng 

from p1 of Assignment 2
h2 - converts gray-level image to binary using a given threshold 
USAGE: ./h2 input_image threshold binary_image

*/

#include "image.h"
#include "imutils.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("USAGE: %s input_image threshold binary_image\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const int threshold(std::stoi(argv[2]));
    const string output_file(argv[3]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }

    ThresholdImage(&img, threshold); 

    if (!WriteImage(output_file, img)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

