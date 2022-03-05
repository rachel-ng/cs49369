/* 

Rachel Ng 

p4 - 

*/

#include "image.h"
#include "util.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;
using namespace ComputerVisionProjects;





unordered_map<int, vector<float>> GetDatabase(string database_name) {
    /*
    get database objects as a map 

    INPUT   string database file name 
    OUTPUT  map[label] = [...] 
    */

    bool debug = true;
    if (debug) cout << "DATABASE " << database_name << endl;
    ifstream input_database(database_name); 
    unordered_map<int, vector<float>> database; 

    string line;
    while(getline(input_database, line)){
        vector<float> db_line;
        string accumulator = ""; 

        for (auto const c : line) {
            if (c != ' ') {
                accumulator += c; 
            }
            else {
                db_line.push_back(stof(accumulator));
                accumulator = ""; 
            }
        }

        database[db_line[0]] = vector<float> (db_line.begin() + 1, db_line.end());
    }

    if (debug) cout << database << endl;
    return database;
}



float PercentDifference(float a, float b, bool percent=false) {
    return abs(a - b) / ((a + b) / 2) * (!percent ? 1 : 100);
}

vector<float> CompareObjects(vector<float> object1, vector<float> object2) {



    // label: center_x center_y 
    //        * min_moment_inertia 
    //        * area 
    //        * roundedness 
    //        orientation_deg

    vector<int> viable {2, 3, 4};

    vector<float> percent_difference; 

    for (int i : viable) {
        float epsilon = min(object1[i],object2[i]) * 0.10;

        if (abs(object1[i] - object2[i]) < epsilon) {
            cout << "\tMATCH";
        }

        cout << "\t" << object1[i] << "\t" << object2[i] << endl;
        cout << "\t\t" << object1[i] - object2[i] << "\t" << epsilon << endl;
        cout << "\t\t" << PercentDifference(object1[i], object2[i], true) << "%";
        cout << endl << endl;

        percent_difference.push_back(PercentDifference(object1[i], object2[i]));
    }

    return percent_difference;
}


int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("Usage: %s file1 db_file file2\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string input_database_file(argv[2]);
    const string output_file(argv[3]);

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }

    string object_model_file = "two_objects_db"; 
    unordered_map<int, vector<float>> object_model_database = GetDatabase(object_model_file);
    unordered_map<int, vector<float>> input_database = GetDatabase(input_database_file);
  
    cout << endl << endl << endl << endl;

    for (auto object : input_database) { 
        for (auto model_object : object_model_database) {
            cout << object << endl;
            cout << model_object << endl;
            vector<float> differences = CompareObjects(object.second, model_object.second);
            cout << endl;
            cout << differences << endl; 
            cout << endl;
            cout << endl;
        } 
    }


    if (!WriteImage(output_file, an_image)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

