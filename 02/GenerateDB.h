#ifndef GENERATE_DB_H_
#define GENERATE_DB_H_


/* 

Rachel Ng 

GenerateDB header file

*/

#include "image.h"
#include "Object.h"
#include <cstdio>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


using namespace std;
using namespace ComputerVisionProjects;

unordered_map<int,Object> GenerateDBEntry(Image *img);

#endif

