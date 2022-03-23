#ifndef IMUTILS_H 
#define IMUTILS_H 

/*

Rachel Ng

image utils - code from previous assignments (thresholding and connected components) and helper functions 

*/

#include "image.h"
#include "Pixel.h"
#include "Object.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


void ThresholdImage(ComputerVisionProjects::Image *img, int);
void HoughSpaceImage(ComputerVisionProjects::Image *img, std::vector<std::vector<int>>);
std::vector<int> NeighborhoodColors(ComputerVisionProjects::Image *img, int, int);
void LabelConnectedRegions(ComputerVisionProjects::Image *img, std::unordered_map<int, std::set<int>> &);
std::unordered_map<int,Object> ConnectedComponents(ComputerVisionProjects::Image *img);
void ImageColors(ComputerVisionProjects::Image *img);

std::unordered_map<int,Object> LabelObjects(ComputerVisionProjects::Image *img);


#endif 
