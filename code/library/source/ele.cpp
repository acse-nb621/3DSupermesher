#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "node.h"
#include "ele.h"

using namespace std;

bool ele::equals(ele b) {
    for(int i = 0; i < 4; i ++){
        if(this->nodes[i] != b.nodes[i]){
            return false;
        }
    }
    return true;
}

void read_eles(string file_name, ele* ele_list){
    ifstream indata;
    indata.open(file_name);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    int ele_count{0};
    info >> ele_count;

    for (int i = 0; i < ele_count; i++){
        string line = "";
        // int marker{0};
        ele curr;
        getline(indata,line);
        istringstream iss(line);
        iss >> curr.nodes[0];
        for(int j = 0; j < 4; j ++){
            iss >> curr.nodes[j];
            curr.nodes[j]++;
        }
        // iss >> curr.marker;
        ele_list[i] = curr;
    }
    indata.close();
}

int count_eles(string filename){
    ifstream indata;
    indata.open(filename);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    int ele_count{0};
    info >> ele_count;
    indata.close();
    return ele_count;
}


void write_eles(vector<ele> &ele_list, string name){
    ofstream outfile(name);
    outfile << ele_list.size()<<"\t14\t0"<<endl;
    for(int i = 0; i < ele_list.size(); i++){
        outfile<< i+1<<"\t"<<ele_list.at(i).nodes[0]<<"\t"<<ele_list.at(i).nodes[1]<<"\t"<<ele_list.at(i).nodes[2]<<"\t"<<ele_list.at(i).nodes[3]<<endl;
    }
}