/*

Rachel Ng

Object class (struct) for connected components

*/

#include "Object.h" 

// constructors 
Object::Object(): label_{-1}, pixels_{std::set<std::pair<int,int>> {}} { }
Object::Object(int label): label_{label}, pixels_{std::set<std::pair<int,int>> {}} { }

/*
insert pixel to pixels_ vector
INPUT   Pixel {x, y}
*/
void Object::AddPixel(std::pair<int, int> pixel) {
    pixels_.insert(pixel);
}



