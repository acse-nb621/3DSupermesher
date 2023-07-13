#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../source/node.h"
#include "../source/face.h"
#include "../source/ele.h"
#include "../tetgen1.6.0/tetgen.h"
using namespace std;



int main()
{
    //get number of nodes & faces for each parent
    int size_parent1_nodes = count_nodes("parent1.node");
    int size_parent2_nodes = count_nodes("parent2.node");
    int size_parent1_faces = count_faces("parent1.face");
    int size_parent2_faces = count_faces("parent2.face");

    node* parent1_nodes = new node[size_parent1_nodes];
    read_nodes("parent1.node",parent1_nodes);
    node* parent2_nodes = new node[size_parent2_nodes];
    read_nodes("parent2.node",parent2_nodes);
    face* parent1_faces = new face[size_parent1_faces];
    read_faces("parent1.face", parent1_faces, size_parent1_faces);
    face* parent2_faces = new face[size_parent2_faces];
    read_faces("parent2.face", parent2_faces, size_parent2_faces);

    //create two mappers, one for each parent's nodes, so we can later assign the right nodes to each face
    int* mapping1 = new int[size_parent1_nodes];
    int* mapping2 = new int[size_parent2_nodes];

    //merge both parents nodes
    vector<node> merged = merge_nodes(parent1_nodes,parent2_nodes,size_parent1_nodes,size_parent2_nodes,mapping1,mapping2);

    //write merged nodes to file
    write_nodes(merged,"merged.node");
    
    vector<face> merged_faces;

    //create hash map to store parent1's faces' nodes
    unordered_map<string,bool> faces_map = {};
    for(int i = 0; i < size_parent1_faces; i ++){
        int temp[3];
        for(int j = 0; j < 3; j++){
            temp[j] = mapping1[parent1_faces[i].nodes[j] - 1];
        }
        sort(temp, temp+3);
        merged_faces.push_back(parent1_faces[i]);
        faces_map[to_string(temp[0])+" "+to_string(temp[1])+" "+to_string(temp[2])] = true;
    }

    //add parent2's faces, taking duplicates into account
    for(int i = 0; i < size_parent2_faces; i++){
        int temp[3] = {mapping2[parent2_faces[i].nodes[0] - 1],mapping2[parent2_faces[i].nodes[1] - 1],mapping2[parent2_faces[i].nodes[2] - 1]};
        sort(temp,temp+3);
        string str = to_string(temp[0]) + " " + to_string(temp[1]) + " " + to_string(temp[2]);
        cout<<"str: "<<str<<endl;
        if(faces_map[str] == 0){
            // cout<<"str"<<endl;
            faces_map[str] = true;
            merged_faces.push_back(parent2_faces[i]);
        }
    }

    write_faces(merged_faces,"merged.face");

    tetgenio in, out;
    tetgenio::facet *f;
    tetgenio::polygon *p;
    in.firstnumber = 1;
    in.numberofpoints = 5;
    in.numberofpoints = merged.size();
    in.pointlist = new REAL[in.numberofpoints * 3];
    for(int i = 0; i <  merged.size(); i++){
        in.pointlist[i*3] = merged[i].coords[0];
        in.pointlist[i*3+1] = merged[i].coords[1];
        in.pointlist[i*3+2] = merged[i].coords[2];
    }
    in.numberoffacets = merged_faces.size();
    in.facetlist = new tetgenio::facet[in.numberoffacets];
    in.facetmarkerlist = new int[in.numberoffacets];
    for(int i = 0; i < merged_faces.size(); i ++){
        f = &in.facetlist[i];
        f->numberofpolygons = 1;
        f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
        f->numberofholes = 0;
        f->holelist = NULL;
        p = &f->polygonlist[0];
        p->numberofvertices = 3;
        p->vertexlist = new int[p->numberofvertices];
        p->vertexlist[0] = merged_faces[i].nodes[0];
        p->vertexlist[1] = merged_faces[i].nodes[1];
        p->vertexlist[2] = merged_faces[i].nodes[2];
        // p->vertexlist[3] = merged_faces[i].nodes[3];
    }

    //commad: g++ node.cpp face.cpp -o driver driver.cpp -L./ -ltet

    in.save_nodes("bar");
    in.save_poly("bar");

    tetgenbehavior b;

    tetrahedralize(&b, &in, &out);
    out.save_nodes("barref");
    out.save_elements("barref");
    out.save_faces("barref");
        
    return 0;
}






  