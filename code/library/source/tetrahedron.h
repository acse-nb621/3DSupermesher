#ifndef TETRAHEDRON
#define TETRAHEDRON
//
// tetrahedron.h: header file for tetrahedron class
//
// Author:  Nourhan Berjawi
//
// A tetrahedron object represents a 3D tetrahedron, defined by 4 nodes.
//
// Constructors:
//
//      tetrahedron()
//          default constructor
// 
//       tetrahedron(double points[12])
//          constructor given coordinates of 4 nodes consecutively as {x0,y0,z0,x1,y1,z1,...}
// 
//       tetrahedron(node n0, node n1, node n2, node n3){
//          constructor given 4 node objects
//
// Public Members:
//
//      nodes
//          this member stores the 4 vertices representing a tetrahedron
//
//      faces
//          this member stores the 4 faces of a tetrahedron and their 3 nodes each
//
//      edges 
//          this member stores the 6 edges of a tetrahedron and their 2 nodes each
//
//      parents
//          integer number representing a bijection of the 2 parent elements of a tetrahedron from each mesh
//      
//      done 
//          2 bits representing which parent element has been set, 00 is parent none, 01 is parent 0, 10 is parent 1, 11 is both

#include <vector>
#include "node.h"
#include <bitset>
using namespace std;

class tetrahedron{
    public:

        // nodes of tetrahedron
        node  nodes[4];

        // bijection of parent elements of tetrahedron
        int parents = 0;

        // two bits indicating which parent has been set
        bitset<2> done = 0;

        // vector of faces
        vector<vector<node>> faces{vector<vector<node>>(4,vector<node>(3))};

        // vector of edges
        vector<vector<node>> edges{vector<vector<node>>(6,vector<node>(2))};

        // default constructor
        tetrahedron(){}

        // constructor given array of 12 points
        tetrahedron(double points[12]){

            for(int i = 0; i < 4; i++){
                nodes[i][0] = points[i*3];
                nodes[i][1] = points[i*3+1];
                nodes[i][2] = points[i*3+2]; 
            }
            for(int j = 0; j < 3; j++){
                faces[0][0][j] = nodes[0][j];
                faces[0][1][j] = nodes[1][j];
                faces[0][2][j] = nodes[2][j];
            }
            for(int i = 0; i < 3; i++){
                faces[1][0][i] = nodes[1][i];
                faces[1][1][i] = nodes[2][i];
                faces[1][2][i] = nodes[3][i];
            }
            for(int i = 0; i < 3; i++){
                faces[2][0][i] = nodes[2][i];
                faces[2][1][i] = nodes[3][i];
                faces[2][2][i] = nodes[0][i];
            }
            for(int i = 0; i < 3; i++){
                faces[3][0][i] = nodes[3][i];
                faces[3][1][i] = nodes[0][i];
                faces[3][2][i] = nodes[1][i];
            }
            int loc[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 2; j++){
                    edges[i][j][0] = nodes[loc[i][j]][0];
                    edges[i][j][1] = nodes[loc[i][j]][1];
                    edges[i][j][2] = nodes[loc[i][j]][2];
                }
            }

        }
        
        // constructor guven 4 nodes
        tetrahedron(node n0, node n1, node n2, node n3){
            nodes[0] = n0;
            nodes[1] = n1;
            nodes[2] = n2;
            nodes[3] = n3;
            for(int j = 0; j < 3; j++){
                faces[0][0][j] = nodes[0][j];
                faces[0][1][j] = nodes[1][j];
                faces[0][2][j] = nodes[2][j];
            }
            for(int i = 0; i < 3; i++){
                faces[1][0][i] = nodes[1][i];
                faces[1][1][i] = nodes[2][i];
                faces[1][2][i] = nodes[3][i];
            }
            for(int i = 0; i < 3; i++){
                faces[2][0][i] = nodes[2][i];
                faces[2][1][i] = nodes[3][i];
                faces[2][2][i] = nodes[0][i];
            }
            for(int i = 0; i < 3; i++){
                faces[3][0][i] = nodes[3][i];
                faces[3][1][i] = nodes[0][i];
                faces[3][2][i] = nodes[1][i];
            }
            int loc[6][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 2; j++){
                    edges[i][j][0] = nodes[loc[i][j]][0];
                    edges[i][j][1] = nodes[loc[i][j]][1];
                    edges[i][j][2] = nodes[loc[i][j]][2];
                }
            }

        }
        ~tetrahedron(){}
};

#endif