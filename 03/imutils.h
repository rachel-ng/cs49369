#include "Object.h"

#ifndef IMUTILS_H 
#define IMUTILS_H 

#include "image.h"
#include "util.h"
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


std::ostream& operator<<(std::ostream& os, const Object& o);

std::ostream& operator<<(std::ostream& os, const Pixel& p);

void ThresholdImage(ComputerVisionProjects::Image *img, int);
void VotingArrayImg(ComputerVisionProjects::Image *img, std::vector<std::vector<int>>);
std::vector<int> NeighborhoodColors(ComputerVisionProjects::Image *img, int, int);
void LabelConnectedRegions(ComputerVisionProjects::Image *img, std::unordered_map<int, std::set<int>> &);
std::unordered_map<int,Object> ConnectedComponents(ComputerVisionProjects::Image *img);
void ImageColors(ComputerVisionProjects::Image *img);

std::unordered_map<int,Object> LabelObjects(ComputerVisionProjects::Image *img);


#endif 
