/* 
Rachel Ng 
*/

#include "imutils.h"


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

