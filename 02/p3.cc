/* 

Rachel Ng 

p3 - compute object attributes and generate a database  

*/

#include "image.h"
#include "Object.h"
#include "GenerateDB.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


using namespace std;
using namespace ComputerVisionProjects;


int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("USAGE: %s labeled_image output_database output_image\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string output_database(argv[2]);
    const string output_file(argv[3]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }


    // process input image
    unordered_map<int,Object> objects_map = GenerateDBEntry(&img); 

    // draw center and orientation of all objects
    for (auto &o : objects_map) {
        pair<int,int> oc = o.second.center();
        img.SetPixel(oc.first, oc.second, 255);

        DrawLine(oc.first, 
                oc.second, 
                round(30 * cos(o.second.orientation()) + oc.first), 
                round(30 * sin(o.second.orientation()) + oc.second), 
                255, 
                &img);
    }

    // write to database 
    ofstream database_file(output_database);
    for (auto &o : objects_map) {
        database_file << o.second.db() << endl;
    }
    database_file.close();

  
    if (false) cout << img.num_rows() << " " << img.num_columns() << "\t" << img.num_gray_levels() << endl;
    if (!WriteImage(output_file, img)){
        cout << "FILE " << output_file << endl;
        return 0;
    }

}

