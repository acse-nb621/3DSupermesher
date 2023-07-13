#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "node.h"

using namespace std;

bool node::equals(node b) {
    if ((this->coords[0] == b[0]) && (this->coords[1] == b[1]) && (this->coords[2] == b[2])){
        return 1;
    }else {
        return 0;
    }
}

// takes two list of nodes and finds their union
vector<node> merge_nodes(node* parent1_nodes, node* parent2_nodes , int size_parent1_nodes , int size_parent2_nodes, int* mapping1, int* mapping2){

    vector<node> nodes;

    for(int i = 0; i < size_parent1_nodes; i ++){
        nodes.push_back(parent1_nodes[i]);
        mapping1[i] = i;
    }
    for(int i = 0; i < size_parent2_nodes; i++){
        bool  flag = true;
        for(int j = 0 ; j < size_parent1_nodes; j++){
            if(parent2_nodes[i].equals(parent1_nodes[j])){
                flag = false;
                mapping2[i] = j; 
                break;
            }
        }
        if(flag){
            nodes.push_back(parent2_nodes[i]);
            mapping2[i] = nodes.size() - 1;
        }
    }
    return nodes;
}

void read_nodes(string file_name, node* node_list){
    ifstream indata;
    indata.open(file_name);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    int node_count= 0;
    info >> node_count;
    for (int i = 0; i < node_count; i++){
        string line = "";
        double coords[3];
        node curr;

        getline(indata,line);
        istringstream iss(line);
        
        iss >> coords[0];
        for(int j = 0; j < 3; j ++){
            iss >> coords[j];
        }
        curr[0] = coords[0];
        curr[1] = coords[1];
        curr[2] = coords[2];
        node_list[i] = curr;
    }
    indata.close();
}

void write_nodes(vector<node> &node_list, string name){
    ofstream outfile(name);
    outfile << node_list.size()<<"\t3\t0\t0"<<endl;
    for(int i = 0; i < node_list.size(); i++){
        outfile<< i+1<<"\t"<<node_list.at(i).coords[0]<<"\t"<<node_list.at(i).coords[1]<<"\t"<<node_list.at(i).coords[2]<<endl;
    }
}

void write_nodes(node* node_list, string name, int node_count){
    ofstream outfile(name);
    outfile <<node_count<<"\t3\t0\t0"<<endl;
    for(int i = 0; i < node_count; i++){
        outfile<< i+1<<"\t"<<node_list[i].coords[0]<<"\t"<<node_list[i].coords[1]<<"\t"<<node_list[i].coords[2]<<endl;
    }
}

int count_nodes(string filename){
    ifstream indata;
    indata.open(filename);
    if(!indata) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    string first_line  = "";
    getline(indata,first_line);
    istringstream info(first_line);
    int node_count{0};
    info >> node_count;
    indata.close();
    return node_count;
}
