/* 

Rachel Ng 

p2 - segment binary image into several connected regions  

*/

#include "image.h"
#include "util.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


using namespace std;
using namespace ComputerVisionProjects;

/*
set image colors 

INPUT   image 
*/
void ImageColors(Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns();

    const int shades = img->num_gray_levels();
    int increments = 100 / shades; 
    set<int> colors;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);
            colors.insert(pixel_color);
            if (pixel_color != 0) {
                pixel_color = 50+(increments * pixel_color);
                img->SetPixel(i, j, pixel_color);
            }
        }
    }

    img->SetNumberGrayLevels(colors.size() - 1);
}

/*
find colors of neighbors

INPUT   image, pixel x and y coordinates 
OUTPUT  vector of colors of neighboring pixels 
*/
vector<int> NeighborhoodColors(Image *img, int px, int py) {
    // INPUTS   image, (x,y) coords of a pixel
    // RETURNS  vector with the color of the neighborhood of a cell

    const int rows = img->num_rows();
    const int cols = img->num_columns();
    const int pixel_color = img->GetPixel(px,py);

    vector<int> nbh_colors;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x = px + i;
            int y = py + j;

            // verify x and y in image, add color to vector if != 0
            if ((-1 < x && x < rows) && (-1 < y && y < cols)) {
                int neighbor_color = img->GetPixel(x,y); 
                if (neighbor_color != 0) {
                    nbh_colors.push_back(neighbor_color);
                }
            }
        }
    }

    return nbh_colors;
}

/*
label all connected components 

INPUT   image, map[label] = {equivalent labels...}
*/
void ConnectedComponents(Image *img, unordered_map<int, set<int>> &equivalence) {
    if (img == nullptr) abort();
    
    const int rows = img->num_rows();
    const int cols = img->num_columns();

    int labels = equivalence.size(); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);

            if (pixel_color != 0) { 
                // get vector of colors of neighbors 
                vector<int> nbh_colors = NeighborhoodColors(img, i, j);

                // make pixel label the lowest label of it's neighbors or 255
                int pixel_label = nbh_colors.size() > 1 ? *min_element(nbh_colors.begin(), nbh_colors.end()) : 255;

                // create new label if no neighbors are labeled 
                if (pixel_label == 255) {
                    labels++;
                    pixel_label = labels;
                    set<int> equiv {pixel_label};
                    equivalence[pixel_label] = equiv; 
                }

                // set pixel to label 
                img->SetPixel(i,j,pixel_label);
                img->SetPixel(i,j,*(equivalence[pixel_label]).begin());

                // add neighbors to equivalence map to easily find lowest equivalent label
                for (auto &n : nbh_colors) {
                    if (n != 0 && n != 255) {
                        equivalence[pixel_label].insert(n);
                        equivalence[n].insert(pixel_label);
                    }
                }

            }
        }
    }
}

int main(int argc, char **argv){
  
    if (argc!=3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string output_file(argv[2]);

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }


    unordered_map<int,set<int>> equivalent_labels;

    // label connected regions 
    ConnectedComponents(&an_image, equivalent_labels); 
    ConnectedComponents(&an_image, equivalent_labels); 
    ImageColors(&an_image);
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }


    return 0;
}

