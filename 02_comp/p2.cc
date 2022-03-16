/* 

Rachel Ng 

p2 - segment binary image into several connected regions  
USAGE: ./p2 binary_image labeled_image

*/

#include "image.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


using namespace std;
using namespace ComputerVisionProjects;

/*
set image colors so we have a visible range 

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

    vector<int> neighborhood_colors;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x = px + i;
            int y = py + j;

            // verify x and y in image, add color to vector if != 0
            if ((-1 < x && x < rows) && (-1 < y && y < cols)) {
                int neighbor_color = img->GetPixel(x,y); 
                if (neighbor_color != 0) {
                    neighborhood_colors.push_back(neighbor_color);
                }
            }
        }
    }

    return neighborhood_colors;
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
                vector<int> neighborhood_colors = NeighborhoodColors(img, i, j);

                // make choose lowest label of neighbors or 255
                int pixel_label = neighborhood_colors.size() > 1 ? *min_element(neighborhood_colors.begin(), neighborhood_colors.end()) : 255;

                // create new label if no neighbors are labeled 
                if (pixel_label == 255) {
                    labels++;
                    pixel_label = labels;
                    set<int> equivalent_set {pixel_label};
                    equivalence[pixel_label] = equivalent_set; 
                }

                img->SetPixel(i,j,pixel_label);
                img->SetPixel(i,j,*(equivalence[pixel_label]).begin());

                // add neighbors to equivalence map to easily find lowest equivalent label
                for (auto &neighbor_color : neighborhood_colors) {
                    if (neighbor_color != 0 && neighbor_color != 255) {
                        equivalence[pixel_label].insert(neighbor_color);
                        equivalence[neighbor_color].insert(pixel_label);
                    }
                }

            }
        }
    }
}

int main(int argc, char **argv){
  
    if (argc!=3) {
        printf("USAGE: %s binary_image labeled_image\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string output_file(argv[2]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }


    unordered_map<int,set<int>> equivalent_labels;

    // label connected regions 
    ConnectedComponents(&img, equivalent_labels); 
    ConnectedComponents(&img, equivalent_labels); 
    ImageColors(&img);


    if (!WriteImage(output_file, img)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }


    return 0;
}

