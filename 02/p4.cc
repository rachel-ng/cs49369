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





unordered_map<int, vector<double>> GetDatabase(string database_name) {
    /*
    get database objects as a map 

    INPUT   string database file name 
    OUTPUT  map[label] = [...] 

      label: center_x center_y 
             * min_moment_inertia 
             * area 
             * roundedness 
             orientation_deg

    */

    bool debug = true;
    if (debug) cout << "DATABASE " << database_name << endl;
    ifstream input_database(database_name); 
    unordered_map<int, vector<double>> database; 

    string line;
    while(getline(input_database, line)){
        vector<double> db_line;
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

        database[db_line[0]] = vector<double> (db_line.begin() + 1, db_line.end());
    }

    if (debug) cout << database << endl;
    return database;
}

double PercentDifference(double a, double b, bool percent=false) {
    return abs(a - b) / ((a + b) / 2) * (!percent ? 1 : 100);
}

vector<double> CompareObjects(vector<double> object1, vector<double> object2) {
    vector<int> viable {2, 3, 4};
    vector<double> percent_difference; 

    for (int i : viable) {
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

    // string object_model_file = "two_objects_db"; 
    string object_model_file = "many_objects_1_db"; 
    unordered_map<int, vector<double>> object_model_database = GetDatabase(object_model_file);
    unordered_map<int, vector<double>> input_database = GetDatabase(input_database_file);
  
    cout << endl << endl << endl << endl;

    for (auto model_object : object_model_database) {
        cout << model_object.first << endl; 
        for (auto object : input_database) { 
            vector<double> differences = CompareObjects(object.second, model_object.second);
            cout << "   " << model_object.first << "\t" << model_object.second << endl;
            cout << " * " << object.first << "\t" << object.second << endl;
            cout << "\t" << differences << endl; 

            
            for (int i = 0; i < differences.size(); i++) {
                if (difference < .05) {
                    cout << "\tMATCH .05\t" << difference[i] << endl;
                }
                else if (difference < .1) {
                    cout << "\tMATCH .1 \t" << difference[i] << endl;
                }
            }
            cout << endl;
        } 
        cout << endl;
        cout << endl;
    }


    if (!WriteImage(output_file, an_image)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

