#ifndef SUPERMESH
#define SUPERMESH
//
// supermesh.h: header file for supermesh class
//
// Author:  Nourhan Berjawi
//
// A supermesh object represents the tetrahedralized union of two parent meshes, and it a mesh itself,
// composed of tetrahedra.
//
// Constructors:
//
//      supermesh(node* p1nodes, ele* p1eles,int elcount1, node* p2nodes, ele* p2eles, int elcount2);
//          constructor that takes 2 lists of node, 2 lists of elements, 2 counts of element
//
// Public Members:
//
//      elements
//          this member stores the tetrahedra that make up the supermesh
//
//      size
//          this member stores the number of tetrahedra that make up the mesh
//

#include "tetrahedron.h"
#include "node.h"
#include "ele.h"
#include "vector"
#include <bitset>
#include <unordered_set>

using namespace std;

class supermesh{
    public: 
        vector<tetrahedron> elements;
        int size;

        supermesh(node* p1nodes, ele* p1eles,int elcount1, node* p2nodes, ele* p2eles, int elcount2);

};

// used for intersect() function: check if face(i) of tetrahedron A lies on a separating plane
bool face(int i, tetrahedron tetA, tetrahedron tetB, double coord[4][4], vector<bitset<4>> &masks, int nf[16]);

// used for intersect() function: check if the edge shared by f0 and f1 lies on a separating plane
bool edge(int f0, int f1,double coord[4][4], vector<bitset<4>> &masks);

// check if any of the points of tetrahedron B lie in tetrahedron A
bool point_inside(vector<bitset<4>> &masks);

// check if tetrahedrons A and B intersect
bool intersect(tetrahedron tetA, tetrahedron tetB);



// used for get_intersection_points(): get the magnitude of a vector
double mag(double v[3]);

// used for get_intersection_points(): get the cross product of 2 vectors
void cross_prod(double a[3], double b[3], double c[3]);

// used for get_intersection_points(): get the directed vector, given two points
void directed_vector(double a[3], double b[3], double c[3]);

// used for get_intersection_points(): get the dot product of 2 vectors
double dot_product(double a[3], double b[3]);

// used for get_intersection_points(): get the signed volume of a tetrahedron
double tet_vol(double a[3], double b[3], double c[3], double d[3]);

// used for get_intersection_points(): adds a vector of doubles to a vetor of vectors only if it is not a repeat
void add_vector(vector<vector<double>> &vec, vector<double> &p);

// used for get_intersection_points(): finds the plucker coordinates of 2 segments, given their endpoints
void plucker(double a[3], double b[3], double p[6]);

// used for get_intersection_points(): finds the side operator of 2 segment, given their endpoints
double side_operator(double p1[3], double q1[3], double p2[3], double q2[3]);

// used for get_intersection_points(): checks if a line intersects a segment, given 2 points on line and segments endpoints
bool line_segment(double a[3], double b[3], double c[3], double d[3]);

// used for get_intersection_points(): checks if 2 segments intersect given their endpoints
bool segment_segment(double a[3], double b[3], double c[3], double d[3]);

// used for get_intersection_points(): gets the intersection of a line and plane, given 2 and 3 points respectively
vector<double> get_line_plane(double pa[3],double pb[3], double p0[3],double p1[3],double p2[3]);

// used for get_intersection_points(): gets the intersection of 2 lines, given 2 points on each
vector<double> get_line_line(double x[3], double w[3], double x_[3], double w_[3]);

// used for get_intersection_points(): gets the intersection point of a line and a triangle
vector<vector<double>> intersect_segment_triangle(double pointA[3], double pointB[3], double point0[3], double point1[3], double point2[2]);

// gets the intersection points of two tetrahedra
vector<vector<double>> get_intersection_points(tetrahedron tetA, tetrahedron tetB);

// returns a vector of tetrahedra, given 2 tetrahedra and their intersection points
vector<tetrahedron> construct_intersection(tetrahedron a, tetrahedron b, vector<vector<double>> points);


#endif
