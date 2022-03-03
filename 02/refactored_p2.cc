/* 

Rachel Ng 

p2 - 

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


void ImageColors(Image *img) {
    bool debug = true;

    const int rows = img->num_rows();
    const int cols = img->num_columns();

    const int shades = img->num_gray_levels();
    int increments = shades > 155 ? 1 : 155 / shades; 
    int base = shades > 155 ? ((254 - 155)/2) : 50;
    unordered_map<int, int> colors;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) { 
            int pixel_color = img->GetPixel(i,j);
            if (pixel_color != 0) {
                if (colors.count(pixel_color) == 0) colors[pixel_color] = colors.size() + 1;
                int new_color =  colors[pixel_color];
                new_color = base + (increments * new_color);
                img->SetPixel(i, j, new_color);
            }
        }
    }

    img->SetNumberGrayLevels(colors.size() - 1);

    if (debug) {
        cout << img->num_rows() << " " << img->num_columns() << "\t" << img->num_gray_levels() << endl;
        cout << "COLORS\t\t" << colors << endl;
    }
}

vector<int> NeighborhoodColors(Image *img, int px, int py) {
    // INPUTS   image, (x,y) coords of a pixel
    // RETURNS  vector with the color of the neighborhood of a cell
    
    bool debug = true;

    const int rows = img->num_rows();
    const int cols = img->num_columns();
    const int pixel_color = img->GetPixel(px,py);

    vector<int> nbh_colors = {};
    vector<pair<int,int>> locations = {{-1,-0}, {0,-1}}; //, {-1,-1}};

    for (auto c : locations) {
        int i = c.first;
        int j = c.second;
        int x = px + i;
        int y = py + j;

        // verify x and y in image, add color to vector if != 0
        if ((-1 < x && x < rows) && (-1 < y && y < cols)) {
            int neighbor_color = img->GetPixel(x,y); 
            if (debug) cout << x << ", " << y << "  " << neighbor_color << "\t\t";
            if (neighbor_color != 0) {
                nbh_colors.push_back(neighbor_color);
            }
        }
    }

    /*
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x = px + i;
            int y = py + j;

            // verify x and y in image, add color to vector if != 0
            if ((-1 < x && x < rows) && (-1 < y && y < cols)) {
                int neighbor_color = img->GetPixel(x,y); 
                if (debug && true) cout << x << ", " << y << "  " << neighbor_color << ((j == 1) ? "\n" : "\t\t");
                if (neighbor_color != 0) {
                    nbh_colors.push_back(neighbor_color);
                }
            }
        }
    }
    */

    if (debug) cout << "\tNEIGHBORS\t" << nbh_colors << "\t";
    // if (debug) cout << nbh_colors.size() ;
    if (debug) cout << endl;

    /*
    if (debug && nbh_colors.size() > 0 && !equal(nbh_colors.begin() + 1, nbh_colors.end(), nbh_colors.begin())) { 
        // cout << "NEIGHBORS\t" << nbh_colors;
    }
    */
    return nbh_colors;
}

void FixNeighbors(Image *img, int px, int py, int color) {
    vector<pair<int,int>> locations = {{-1,-0}, {0,-1}, {-1,-1}};
    const int rows = img->num_rows();
    const int cols = img->num_columns();
    cout << "ADJUST NEIGHBORS\t" << endl;

    for (auto c : locations) {
        int i = c.first;
        int j = c.second;
        int x = px + i;
        int y = py + j;
        int neighbor_color = img->GetPixel(x,y); 

        // verify x and y in image and update color 
        if ((-1 < x && x < rows) && (-1 < y && y < cols) && neighbor_color != 0 && neighbor_color != 255) {
            if (true) cout << "(" << x << ", " << y << ")  " << neighbor_color << " -> " << color << "\t\t";
            img->SetPixel(x,y,color);
        }
    }
}


void ConnectedComponents(Image *img, unordered_map<int, set<int>> &equivalence) {
    if (img == nullptr) abort();
    
    bool debug = true; 
    const int rows = img->num_rows();
    const int cols = img->num_columns();

    int labels = equivalence.size(); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);

            if (pixel_color != 0) { 
                if (debug) cout << "(" << i << ", " << j << ")\t" << pixel_color << endl;
                vector<int> nbh_colors = NeighborhoodColors(img, i, j);

                int pixel_label = nbh_colors.size() > 0 ? min(pixel_color, *min_element(nbh_colors.begin(), nbh_colors.end())) : pixel_color;

                if (debug) cout << "PIXEL LABEL\t" << pixel_label;

                if (pixel_color == 255) {
                    labels++;
                    pixel_label = labels;
                    set<int> equiv {pixel_label};
                    equivalence[pixel_label] = equiv; 

                    if (debug) cout << " -> " << pixel_label << "\t" << *(equivalence[pixel_label]).begin();
                }

                if (debug) cout << endl;
                int equivalent_label =  *(equivalence[pixel_label]).begin();

                /*
                if (equivalent_label != pixel_label && !equal(nbh_colors.begin() + 1, nbh_colors.end(), nbh_colors.begin())) {
                    FixNeighbors(img, i, j, equivalent_label);
                }
                */

                equivalence[equivalent_label].insert(pixel_label);

                img->SetPixel(i,j,equivalent_label);

                for (auto &n : nbh_colors) {
                    if (n != 0 && n != 255 && equivalence[equivalent_label].count(n) == 0){
                        if (pixel_label != n) {
                            equivalence[pixel_label].insert(n);
                            equivalence[n].insert(pixel_label);
                            if (pixel_label != equivalent_label) {
                                equivalence[equivalent_label].insert(n);
                                equivalence[n].insert(equivalent_label);
                                cout << equivalent_label << ": " << equivalence[equivalent_label] << endl;
                            }
                            cout << pixel_label << ": " << equivalence[pixel_label] << endl;
                            cout << n<< ": " << equivalence[n] << endl;
                        }
                    }
                }
            cout << endl << endl;
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

    if (an_image.num_gray_levels() != 1) {
        cout <<"FILE " << input_file << " not binary" << endl;
        return 0;
    }

    bool debug = true; 

    unordered_map<int,set<int>> equivalent_labels;

    if (debug) cout << "PASS 1" << endl;
    ConnectedComponents(&an_image, equivalent_labels); 
    // ImageColors(&an_image);
    if (!WriteImage("1_" + output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    if (debug) cout << "PASS 2" << endl;
    ConnectedComponents(&an_image, equivalent_labels); 
    ImageColors(&an_image);
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    if (true) cout << equivalent_labels << endl;

    if (!ReadImage("1_" + output_file, &an_image)) {
        cout <<"Can't open file " << "1_" + input_file << endl;
        return 0;
    }
    ImageColors(&an_image);
    if (!WriteImage("1_" + output_file, an_image)){
        cout << "Can't write to file " << "1_" + output_file << endl;
        return 0;
    }

    return 0;
}

