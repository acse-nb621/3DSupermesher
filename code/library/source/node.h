#ifndef NODE
#define NODE
//
// node.h: header file for node class
//
// Author:  Nourhan Berjawi
//
// A node object represents a 3D point, defined by coordinates.
// These nodes are outputted by TetGen's tetrahedralize method into a .node file.
//
// Constructors:
//
//      node()
//          default constructor
// 
//       node(double x_, double y_, double z_)
//          constructor given coordinates of node
//
// Public Member Functions:
//
//      bool equals(node b);
//          this function checks if the node object is equal to another one b.
//          input:  node type
//          output: boolean value
//      
//      the indexing [ ] operator has been overloaded to access the memeber coords
//
// Public Members:
//
//      double coords [3]
//          this member stores doubles representing the 3D coordinates of a point
//


#include <iostream>
#include <vector>
#include <string>

using namespace std;

class node {
    public:
        //defining and initializing the x, y, and z components
        double coords[3] = {0};
        bool parent;

        //default node constructor 
        node(){}

        //node constructor given 3 double values
        node(double x_, double y_, double z_) {
            coords[0] = x_;
            coords[1] = y_;
            coords[2] = z_;
        }

        //function to print a node
        void print(){cout<<coords[0]<<" "<<coords[1]<<" "<<coords[2]<<endl;}

        //function to compare 2 nodes
        bool equals(node b);

        //overloading indexing constructor
        double operator [](int i) const    {return coords[i];}
        double & operator [](int i) {return coords[i];}

        ~node(){}

};



// function to read nodes from a .node file.
// input: 
//      - file_name: the name of the .node file as a string 
//      - node_list: pointer to array of node objects
//      - node_count: size of array
// output:
//      - none

// function to read nodes from a .node file.
void read_nodes(string file_name, node* node_list);


// function to merge two node arrays.
// arguments: 
//      - parent1_nodes:  array of nodes of the first parent mesh
//      - size_parent1_nodes:   integer number of nodes of first parent mesh
//      - mapping1: array that maps the nodes in the first parent mesh to the nodes in the supermesh
//      - parent2_nodes:  array of nodes of the second parentmesh
//      - size_parent2_nodes:   integer number of nodes of second parent mesh
//      - mapping2: array that maps the nodes in the second parent mesh to the nodes in the supermesh
// return value:
//      - vector of nodes

// function to merge two node arrays.
vector<node> merge_nodes(node* parent1_nodes, node* parent2_nodes , int size_parent1_nodes , int size_parent2_nodes, int* mapping1, int* mapping2);


// function to write nodes to a .node file.
// arguments: 
//      - node_list: vector of nodes to write to a file
//      - name: the name of the .node file as a string 
// return value:
//      - none

// function to write nodes to a .node file.
void write_nodes(vector<node> &node_list, string name);

// function to return the integer length of a node array
int len(node* node_list);

// function to return the number of nodes in a .node file, given its name
int count_nodes(string filename);

#endif