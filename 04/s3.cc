/* 

Rachel Ng 

s3 - 

USAGE: ./s

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


double det(double a, double b, double c, double d) { 
    return (a*d) - (c*b);
}

double det() {

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


    std::ofstream output_stream(directions_file);
    for (std::vector<double> norm : norms) { 
        output_stream << norm[0] << " " << norm[1] << " " << norm[2] << std::endl;
        std::cout << norm[0] << " " << norm[1] << " " << norm[2] << std::endl;
    }
    output_stream.close();
}


