#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "node.h"
#include "face.h"

using namespace std;


void read_faces(string file_name, face* face_list, int face_count){
    ifstream indata;
    indata.open(file_name);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    info >> face_count;

    for (int i = 0; i < face_count; i++){
        string line = "";
        int marker{0};
        face curr;
        getline(indata,line);
        istringstream iss(line);
        iss >> curr.nodes[0];
        for(int j = 0; j < 3; j ++){
            iss >> curr.nodes[j];
        }
        // iss >> curr.marker;
        face_list[i] = curr;
    }
    indata.close();
}

int count_faces(string filename){
    ifstream indata;
    indata.open(filename);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    int face_count{0};
    info >> face_count;
    indata.close();
    return face_count;
}


void write_faces(vector<face> &face_list, string name){
    ofstream outfile(name);
    outfile << face_list.size()<<"\t1"<<endl;
    for(int i = 0; i < face_list.size(); i++){
        outfile<< i+1<<"\t"<<face_list.at(i).nodes[0]<<"\t"<<face_list.at(i).nodes[1]<<"\t"<<face_list.at(i).nodes[2]<<endl;
    }
}