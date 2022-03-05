/* 

Rachel Ng 

generate database

*/

#include "GenerateDB.h"

/*
generate map for database 

INPUT   image
OUTPUT  map[label] = Object()
*/
unordered_map<int,Object> GenerateDBEntry(Image *img) {
    if (img == nullptr) abort();

    const int rows = img->num_rows();
    const int cols = img->num_columns();
    unordered_map<int,Object> objects;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);
            if (pixel_color != 0) { 
                if (objects.count(pixel_color) == 0) objects[pixel_color] = Object(pixel_color); 
                objects[pixel_color].AddPixel({i,j});
            }
        }
    }

    // get second moment of object
    // updates center 
    // set minimum moment of inertia, roundness, and orientation
    for (auto &o : objects) {
        o.second.moment_2();
    }

    return objects;
}

