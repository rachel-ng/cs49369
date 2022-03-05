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




vector<vector<string>> GetDatabase(string database_name) {
    ifstream input_database(database_name); 

    string line;
    vector<vector<string>> database;

    while(std::getline(input_database, line)){
        vector<string> db_line;
        string accumulator = ""; 
        for (auto const c : line) {
            if (c != ' ') {
                accumulator += c; 
            }
            else {
                db_line.push_back(accumulator);
                accumulator = ""; 
            }
        }

        database.push_back(db_line);
        cout << db_line << endl;
    }
    return database;
}


int main(int argc, char **argv){
  
    if (argc!=4) {
        printf("Usage: %s file1 db_file file2\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]);
    const string input_database(argv[2]);
    const string output_file(argv[3]);

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"FILE " << input_file << endl;
        return 0;
    }

    string object_model_file = "two_objects_db"; 
    GetDatabase(object_model_file);

    GetDatabase(input_database);
  
    if (!WriteImage(output_file, an_image)){
        cout << "FILE " << output_file << endl;
        return 0;
    }
}

