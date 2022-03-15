#ifndef OBJECT_H_
#define OBJECT_H_

#include<iostream>
#include <set>

struct Object{
	Object();
	Object(int label);
	int label_; 
	std::set<std::pair<int, int>> pixels_;
	std::pair<int, int> center_;

	void AddPixel(std::pair<int, int>);
};


#endif

