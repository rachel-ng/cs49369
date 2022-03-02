/* 

Rachel Ng 

p2 - 

*/

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>


using namespace std;
using namespace ComputerVisionProjects;

void printVint(vector<int> v) {
    // cout << "vec<int>\t"; 
    for (const int &i : v) {
        cout << i << "  ";
    }
}

void printSint(set<int> s) {
    // cout << "set<int>\t";  
    for (const int i : s) {
        cout << i << "  ";
    }
}

void ImageColors(Image *img) {
    bool debug = true;

    const int rows = img->num_rows();
    const int cols = img->num_columns();

    const int shades = img->num_gray_levels();
    int increments = 200 / shades; 
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

    if (debug) {
        cout << img->num_rows() << " " << img->num_columns() << "\t" << img->num_gray_levels() << endl;
        cout << "COLORS\t\t" ;
        printSint(colors);
        cout << endl;
    }
}

vector<int> NeighborhoodColors(Image *img, int px, int py) {
    // INPUTS   image, (x,y) coords of a pixel
    // RETURNS  vector with the color of the neighborhood of a cell
    
    bool debug = true;

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
                if (debug && true) cout << x << ", " << y << "  " << neighbor_color << ((j == 1) ? "\n" : "\t\t");
                if (neighbor_color != 0) {
                    nbh_colors.push_back(neighbor_color);
                }
            }
        }
    }

    if (debug && !equal(nbh_colors.begin() + 1, nbh_colors.end(), nbh_colors.begin())) { 
        cout << "NEIGHBORS\t";
        printVint(nbh_colors);
    }

    return nbh_colors;
}


void ConnectedComponents(Image *img, unordered_map<int, set<int>> &equivalence) {
    if (img == nullptr) abort();
    
    const int rows = img->num_rows();
    const int cols = img->num_columns();

    int labels = equivalence.size(); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);

            if (pixel_color != 0) { 
                cout << endl << "(" << i << ", " << j << ")\t" << pixel_color << endl;
                vector<int> nbh_colors = NeighborhoodColors(img, i, j);
                // cout << endl; 

                int pixel_label = nbh_colors.size() > 1 ? *min_element(nbh_colors.begin(), nbh_colors.end()) : 255;

                if (pixel_label == 255) {
                    cout << "PIXEL LABEL\t" << pixel_label;
                    labels++;
                    pixel_label = labels;
                    set<int> equiv {pixel_label};
                    equivalence[pixel_label] = equiv; 

                    cout << " -> " << pixel_label << endl;
                }

                img->SetPixel(i,j,pixel_label);
                img->SetPixel(i,j,*(equivalence[pixel_label]).begin());

                for (auto &n : nbh_colors) {
                    if (n != 0 && n != 255) {
                        equivalence[pixel_label].insert(n);
                        equivalence[n].insert(pixel_label);
                    }
                }

                cout << endl;
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

    cout << "PASS 1" << endl << endl;
    ConnectedComponents(&an_image, equivalent_labels); 
    // ImageColors(&an_image);
    if (!WriteImage("1_" + output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }


    cout << "PASS 2" << endl << endl;
    ConnectedComponents(&an_image, equivalent_labels); 
    ImageColors(&an_image);
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }


    if (false) {
        for (auto const &i : equivalent_labels) {
            cout << i.first << "\t"; 
            printSint(i.second); 
            cout << endl;
        }
    }

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

