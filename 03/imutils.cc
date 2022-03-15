/* 

Rachel Ng 

*/

#include "imutils.h"

std::ostream& operator<<(std::ostream& os, const Pixel& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


std::ostream& operator<<(std::ostream& os, const Object& o) {
    os << o.label_ << " " << o.center_ << "\t" << o.pixels_ << "\n"; 
    return os;
}



/*
binarize image using a threshold

INPUT   image to threshold, int to use as threshold 
*/
void ThresholdImage(ComputerVisionProjects::Image *img, int threshold) {
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


/*
allocate img size and gray levels
draw voting array on image for visualization 

INPUT   image to draw on
        voting array to draw 
*/
void HoughSpaceImage(ComputerVisionProjects::Image *img, std::vector<std::vector<int>> voting_array) {
    img->AllocateSpaceAndSetSize(voting_array.size(), voting_array[0].size());
    img->SetNumberGrayLevels(255);

    for (int i = 0; i < voting_array.size(); i++) {
        for (int j = 0; j < voting_array[0].size(); j++){ 
            img->SetPixel(i, j, voting_array[i][j]);
        }
    }
}


/*
print set of colors in image

INPUT   image to look at 
*/
void ImageColors(ComputerVisionProjects::Image *img) {
    const int rows = img->num_rows();
    const int cols = img->num_columns();
    std::set<int> colors;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            colors.insert(img->GetPixel(i,j));
        }
    }

	std::cout << colors << std::endl;
}



/*
find colors of neighbors

INPUT   image, pixel x and y coordinates
OUTPUT  vector of colors of neighboring pixels
*/
std::vector<int> NeighborhoodColors(ComputerVisionProjects::Image *img, int px, int py) {
    const int rows = img->num_rows();
    const int cols = img->num_columns();
    const int pixel_color = img->GetPixel(px,py);

    std::vector<int> neighborhood_colors;

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
void LabelConnectedRegions(ComputerVisionProjects::Image *img, std::unordered_map<int, std::set<int>> &equivalence) {
    if (img == nullptr) abort();

    const int rows = img->num_rows();
    const int cols = img->num_columns();

    int labels = equivalence.size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){
            int pixel_color = img->GetPixel(i,j);

            if (pixel_color != 0) {
                std::vector<int> neighborhood_colors = NeighborhoodColors(img, i, j);

                // make choose lowest label of neighbors or 255
                int pixel_label = neighborhood_colors.size() > 1 ? *min_element(neighborhood_colors.begin(), neighborhood_colors.end()) : 255;

                // create new label if no neighbors are labeled
                if (pixel_label == 255) {
                    labels++;
                    pixel_label = labels;
                    std::set<int> equivalent_set {pixel_label};
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

/*
label / create all objects
add pixels to array 

INPUT   image 
OUTPUT  objects[label] = object
*/
std::unordered_map<int,Object> LabelObjects(ComputerVisionProjects::Image *img) {
	const int rows = img->num_rows();
    const int cols = img->num_columns();
    std::unordered_map<int,Object> objects;

	for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){ 
            int pixel_color = img->GetPixel(i,j);
            if (pixel_color != 0) { 
                if (objects.count(pixel_color) == 0) objects[pixel_color] = Object(pixel_color); 
                objects[pixel_color].AddPixel({i,j});
            }
        }
    }

	return objects;
}

/*
function to run connected components on an image 

INPUT   image to run on 
OUTPUT  objects[label] = object
*/

std::unordered_map<int,Object> ConnectedComponents(ComputerVisionProjects::Image *img) {

    std::unordered_map<int,std::set<int>> equivalent_labels;

    // label connected regions
    LabelConnectedRegions(img, equivalent_labels);
    LabelConnectedRegions(img, equivalent_labels);

	ImageColors(img);

	std::unordered_map<int,Object> objects = LabelObjects(img);
    return objects; 
}





