/* 

Rachel Ng 

p4 - object recognition using labeled image and a given database
USAGE: ./p4 labeled_image database_file output_image

*/

#include "image.h"
#include "Object.h"
#include "GenerateDB.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;
using namespace ComputerVisionProjects;

/*
get database objects as a map 

INPUT   string - database file name 
OUTPUT  map[label] = [...] 
*/
unordered_map<int, vector<double>> GetDatabase(string database_name) {
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

    return database;
}


/*
returns percent difference

INPUT   double a, double b, return as percentage
OUTPUT  percent difference as a double  
*/
double PercentDifference(double a, double b, bool percent=false) {
    return abs(a - b) / ((a + b) / 2) * (!percent ? 1 : 100);
}


/*
compare 2 objects, return percent differences 

INPUT   2 objects to compare, vector of indices to check of the objects
OUTPUT  vector of percent differences of values we're interested in
*/
vector<double> CompareObjects(vector<double> object1, vector<double> object2, vector<int> viable={4}) {
    vector<double> percent_difference; 

    for (int i : viable) {
        percent_difference.push_back(PercentDifference(object1[i], object2[i]));
    }

    return percent_difference;
}


int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("USAGE: %s labeled_image database_file output_image\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string input_database_file(argv[2]);
    const string output_file(argv[3]);

    Image img;

    if (!ReadImage(input_file, &img)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }


    // process input image 
    unordered_map<int,Object> objects_map = GenerateDBEntry(&img); 
    string output_database = input_file + "_db"; 

    // write to database 
    ofstream database_file(output_database);
    for (auto &o : objects_map) {
        database_file << o.second.db() << endl;
    }
    database_file.close();

  
    // get both databases 
    unordered_map<int, vector<double>> object_model_database = GetDatabase(input_database_file);
    unordered_map<int, vector<double>> generated_database = GetDatabase(output_database);

    for (auto object : generated_database) { 
        bool identified = false;
        for (auto model_object : object_model_database) {
            vector<double> differences = CompareObjects(object.second, model_object.second);
            for (int i = 0; i < differences.size(); i++) {
                // if object is identified draw center and orientation  
                if (differences[i] < .05) {
                    img.SetPixel(object.second[0], object.second[1], 255);

                    DrawLine(object.second[0],
                        object.second[1], 
                        round(30 * cos(object.second[5] * M_PI / 180.0) + object.second[0]), 
                        round(30 * sin(object.second[5] * M_PI / 180.0) + object.second[1]), 
                        255, 
                        &img);
                    identified = true;
                }
                if (identified) break;
            }
            if (identified) break;
        } 
    }


    if (!WriteImage(output_file, img)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

