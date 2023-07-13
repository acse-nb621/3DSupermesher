#include "supermesh.h"
#include "tetrahedron.h"
#include "node.h"
#include "ele.h"
#include <vector>
#include <bitset>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

bitset<4> bit15(15);
bitset<4> bit1(1);
bitset<4> bit2(2);
bitset<4> bit4(4);
bitset<4> bit0(0);
bitset<4> bit8(8);

//  HELPER FUNCTIONS START HERE
    void add_vector(vector<vector<double>> &vec, vector<double> &p){
        for(int i = 0; i < vec.size(); i++){
            if(vec[i] == p){ // if point is already in vector, break
                return;
            }
        }
        vec.push_back(p);
    }

    double mag(double v[3]){
        return sqrt(pow(v[0],2)+pow(v[1],2)+pow(v[2],2));
    }

    void cross_prod(double a[3], double b[3], double c[3]){
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
    }

    void directed_vector(double a[3], double b[3], double c[3]){
        for(int i = 0; i < 3; i++){
            c[i] = b[i] - a[i];
        }
    }

    double dot_product(double a[3], double b[3]){
        double sum = 0.0;
        for(int i = 0; i < 3; i++){
            sum += a[i]*b[i];
        }
        return sum;
    }

    double tet_vol(double a[3], double b[3], double c[3], double d[3]){
        // signed volume is defined by: ad . ( ab x ac )
        double ad[3];
        double ab[3];
        double ac[3];
        directed_vector(a,d,ad);
        directed_vector(a,b,ab);
        directed_vector(a,c,ac);
        double abxac[3];
        cross_prod(ab,ac,abxac);
        double dot = dot_product(ad,abxac);
        return (dot/6);
    }

    bool face(int i, tetrahedron tetA, tetrahedron tetB, double coord[4][4], vector<bitset<4>> &masks){
        double e0[3];//first edge
        directed_vector(tetA.faces[i][0].coords,tetA.faces[i][1].coords,e0);
        double e1[3];//second edge
        directed_vector(tetA.faces[i][0].coords,tetA.faces[i][2].coords,e1);
        double n[3]; //normal vector to plane
        cross_prod(e0,e1,n);
        double ref_vector[3];

        directed_vector(tetA.faces[i][0].coords,tetA.faces[(i+1)%4][2].coords,ref_vector);
        if(dot_product(ref_vector,n)>0){ // check if normal points into the tetrahedron or out of it
            n[0] *= -1; // if it points into it, take the opposite of the normal vector
            n[1] *= -1;
            n[2] *= -1;
        }

        for(int j = 0 ; j < 4; j ++){ //loop over all the points of tetrahedron B and check on which side of the plane they lie
            double temp[3];
            directed_vector(tetA.faces[i][0].coords,tetB.nodes[j].coords,temp); //vector B[j] - A.face(i)[0] 
            coord[i][j] = dot_product(n,temp); // dot product with normal

            if(coord[i][j]>0){ // if true, point is on outer side of plane
                masks[i] |= pow(2,j);
            }
        }
        if(masks[i] == 15){ // if all points on outer side of tetrahedron, then face(i) lies on a separating plane
            return true;
        }
        return false;
    }

    bool edge(int f0, int f1,double coord[4][4], vector<bitset<4>> &masks){
        bitset<4> maskf0 = masks[f0];
        bitset<4> maskf1 = masks[f1];
        if((maskf0 | maskf1) != bit15){ //if theres  a vertex on b in the space between the two faces i and j, return false
            return false;
        }
        maskf0 = masks[f0] & (masks[f0] ^ masks[f1]); // exclude vertices in ++
        maskf1 = masks[f1] & (masks[f0] ^ masks[f1]); 

        // for edge 0,1
        if (((maskf0 & bit0) == bit1) && ((maskf1 & bit2) == bit2) && ((((coord[f0][1] * coord[f1][0])-(coord[f0][0]*coord[f1][1]))>0))){
            return false;
        }

        //for edge 02
        if (((maskf0 & bit1) == bit1) && ((maskf1 & bit4) == bit4) && ((((coord[f0][2] * coord[f1][0])-(coord[f0][0]*coord[f1][2]))>0))){
            return false;
        }

        //for edge 03
        if (((maskf0 & bit1) == bit1) && ((maskf1 & bit8) == bit8) && ((((coord[f0][3] * coord[f1][0])-(coord[f0][0]*coord[f1][3]))>0))){
            return false;
        }

        //for edge 12
        if (((maskf0 & bit2) == bit2) && ((maskf1 & bit4) == bit4) && ((((coord[f0][2] * coord[f1][1])-(coord[f0][1]*coord[f1][2]))>0))){
            return false;
        }
        //for edge 13
        if (((maskf0 & bit2) == bit2) && ((maskf1 & bit8) == bit8) && ((((coord[f0][3] * coord[f1][1])-(coord[f0][1]*coord[f1][3]))>0))){
            return false;
        }

        //for edge 23
        if (((maskf0 & bit4) == bit4) && ((maskf1 & bit8) == bit8) && ((((coord[f0][3] * coord[f1][2])-(coord[f0][2]*coord[f1][3]))>0))){
            return false;
        }
        return true;
    }

    bool point_inside(vector<bitset<4>> &masks){
        bitset<4> bit15(15);
        if ((((masks[0] | masks[1]) | masks[2]) | masks[3]) != bit15){ // if point is on inner side of all faces
            return true;                                               // then it's in the tetrahedron
        }
        return false;
    }

    bool intersect(tetrahedron tetA, tetrahedron tetB){
        double coord[4][4];
        vector<bitset<4>> masks(4);
        if(face(0,tetA, tetB, coord, masks)){ // check is face 0 of A separates the two
            return false;
        }
        if(face(1,tetA, tetB,coord, masks)){ // check is face 1 of A separates the two
            return false;
        }
        if(edge(0,1,coord, masks)){ // check is segment shared by faces 0 and 1 of A lies on a separating plane
            return false;
        }
        if(face(2,tetA,tetB,coord, masks)){ // check is face 2 of A separates the two
            return false;
        }
        if(edge(0,2,coord, masks)){// check is segment shared by faces 0 and 2 of A lies on a separating plane
            return false;
        }
        if(edge(1,2,coord, masks)){// check is segment shared by faces 1 and 2 of A lies on a separating plane
            return false;
        }
        if(face(3, tetA, tetB,coord, masks)){ // check is face 3 of A separates the two
            return false;
        }

        if(edge(0,3,coord, masks)){// check is segment shared by faces 0 and 3 of A lies on a separating plane
            return false;
        }

        if(edge(1,3,coord, masks)){// check is segment shared by faces 1 and 3 of A lies on a separating plane
            return false;
        }

        if(edge(2,3,coord, masks)){// check is segment shared by faces 2 and 3 of A lies on a separating plane
            return false;
        }

        if(point_inside(masks)){
            return true;
        }
        double coord1[4][4];
        vector<bitset<4>> masks1(4);

        if(face(0, tetB, tetA,coord1, masks1)){ // check is face 0 of B separates the two
            return false;
        }

        if(face(1, tetB, tetA,coord1, masks1)){ // check is face 1 of B separates the two
            return false;
        }

        if(face(2, tetB, tetA,coord1, masks1)){ // check is face 2 of B separates the two
            return false;
        }

        if(face(3, tetB, tetA,coord1, masks1)){ // check is face 3 of B separates the two
            return false;
        }

        return true;
    }

    void plucker(double p[3], double q[3], double L[6]){
        // check: https://members.loria.fr/SLazard/ARC-Visi3D/Pant-project/files/plucker.html
        L[0] = p[0]*q[1] - q[0]*p[1];
        L[1] = p[0]*q[2] - q[0]*p[2];
        L[2] = p[0] - q[0];
        L[3] = p[1]*q[2] - q[1]*p[2];
        L[4] = p[2] - q[2];
        L[5] = q[1] - p[1];
    }

    double side_operator(double p1[3], double q1[3], double p2[3], double q2[3]){
        // check: https://members.loria.fr/SLazard/ARC-Visi3D/Pant-project/files/plucker.html
        double a[6];
        double b[6];
        plucker(p1,q1,a);
        plucker(p2,q2,b);
        return (a[0]*b[4] + a[1]*b[5] + a[2]*b[3] + a[3]*b[2] + a[4]*b[0] + a[5]*b[1]);
    }

    bool line_segment(double a[3], double b[3], double c[3], double d[3]){
        //checking line segment intersection using plucker coordinates method
        // link: https://members.loria.fr/SLazard/ARC-Visi3D/Pant-project/files/Line_Segment_Line.html
        double p[3]; 
        double cd[3];
        double ca[3];
        double cb[3];
        double n[3];
        directed_vector(c,d,cd);
        directed_vector(c,a,ca);
        directed_vector(c,b,cb);
        cross_prod(cd,ca,n);
        if((n[0] == 0) && (n[1] == 0) && (n[2] == 0)){
            cross_prod(cd,cb,n);
            if((n[0]==0) && (n[1]==0) && (n[2]==0)){
                return true;
            }
        }
        p[0] = a[0]+n[0];
        p[1] = a[1]+n[1];
        p[2] = a[2]+n[2];

        double s1 = side_operator(a,b,p,c);
        double s2 = side_operator(a,b,d,p);

        return ((s1*s2)>=0);
    }

    bool segment_segment(double a[3], double b[3], double c[3], double d[3]){
        if(!line_segment(a,b,c,d)){ // if line of segment 1 intersects segment 2, there could be an intersection
            return false;
        }
        if(!line_segment(c,d,a,b)){ // above true and line of segment 2 intersects segment 1, there is an intersection
            return false;
        }
        return true;
    }

    vector<double> get_line_plane(double pa[3],double pb[3], double p0[3],double p1[3],double p2[3]){

        double d[3];
        double p0p1[3];
        double p0p2[3];
        double ap0[3];
        directed_vector(pa,pb,d); // direction vector of segment
        directed_vector(p0,p1,p0p1); // vector on plane
        directed_vector(p0,p2,p0p2); // vector on plane
        directed_vector(pa,p0,ap0); // direction vector from segment to point on plane
        double n[3];
        cross_prod(p0p1,p0p2,n); // normal to plane

        double t = (dot_product(n,ap0))/(dot_product(n,d)); // scale factor

        vector<double> point;
        point.push_back(pa[0]+t*d[0]);
        point.push_back(pa[1]+t*d[1]);
        point.push_back(pa[2]+t*d[2]);

        return point;
    }

    bool point_on_line(double p[3], double x1[3], double x2[3]){
        double x1p[3];
        directed_vector(x1,p,x1p); // vector from point to line
        double x2p[3];
        directed_vector(x2,p,x2p); // other vector from point to line
        double x1x2[3];
        directed_vector(x1,x2,x1x2); // direction vector of line
        double n[3];
        cross_prod(x1p,x2p,n);
        double distance = mag(n)/mag(x1x2); // check distance between point and line
        if(distance < pow(10,-4)){
            return true;
        }
        return false;
        
    }

    vector<double> get_line_line(double c[3], double w[3], double d[3], double w_[3]){
        double e[3];
        directed_vector(c,w,e);
        double f[3];
        directed_vector(d,w_,f);
        double g[3];
        directed_vector(c,d,g);
        if(point_on_line(c,d,w_)){ // check if any point on line 1 is on line 2
            vector<double> vc{c[0],c[1],c[2]};
            return vc;
        }
        if(point_on_line(d,c,w)){ // check if any point on line 2 is on line 1
            vector<double> vd{d[0],d[1],d[2]};
            return vd;
        }

        double fxg[3];
        cross_prod(f,g,fxg);
        double fe[3];
        cross_prod(f,e,fe);
        int sign = -1;
        if(dot_product(fxg,fe)>0){
            sign = 1;
        }
        double scalar = sign*(mag(fxg))/(mag(fe));
        vector<double> M{c[0]+scalar*e[0],c[1]+scalar*e[1],c[2]+scalar*e[2]};
        return M;
    }

    vector<vector<double>> intersect_segment_triangle(double pointA[3], double pointB[3], double point0[3], double point1[3], double point2[2]){
        vector<vector<double>> points;
        double AB[3];
        directed_vector(pointA, pointB,AB);
        // use signed volumes to check whether segment intersects triangle
        double v1 = tet_vol(point0,point1,pointA,pointB);
        double v2 = tet_vol(point1,point2,pointA,pointB);
        double v3 = tet_vol(point2,point0,pointA,pointB);
        double v4 = tet_vol(point0,point1,point2,pointA);
        double v5 = tet_vol(point0,point1,point2,pointB);
        if(v4*v5>0){ 
            return points;
        }

        if((v1 == 0) && (v2 == 0) && (v3 ==0)){ //coplanar segment and triangle

            if(segment_segment(pointA,pointB,point0,point1)){
                vector<double> ab01 = get_line_line(pointA,pointB,point0,point1);
                if(!ab01.empty()){
                    points.push_back(ab01);
                }
                
            }
            if(segment_segment(pointA,pointB,point1,point2)){
                vector<double> ab12 = get_line_line(pointA,pointB,point1,point2);
                if(!ab12.empty()){
                    points.push_back(ab12);
                }
            }
            if(segment_segment(pointA,pointB,point2,point0)){
                vector<double> ab20 = get_line_line(pointA,pointB,point2,point0);
                if(!ab20.empty()){
                    points.push_back(ab20);
                }
            }
        }else{ // non-coplanar segment and triangle
            int count0 = 0;
            int countpos = 0;
            int countneg = 0;
            if(v1==0){
                count0++;
            }else if(v1>0){
                countpos++;
            }else{
                countneg++;
            }
            if(v2==0){
                count0++;
            }else if(v2>0){
                countpos++;
            }else{
                countneg++;
            }
            if(v3==0){
                count0++;
            }else if(v3>0){
                countpos++;
            }else{
                countneg++;
            }
            if((countneg == 3)||(countpos == 3)){ // the two tetrahedra intersect only if all three volumes have the same sign
                vector<double> x = get_line_plane(pointA, pointB, point0, point1, point2);
                points.push_back(x);
            }
        }
        return points;
    }

//  HELPER FUNCTIONS END HERE



//  PRIMARY FUNCTIONS START HERE

    // gets the points of intersection of 2 tetrahedra
    vector<vector<double>> get_intersection_points(tetrahedron tetA, tetrahedron tetB){
        // 6 segments of tetB intersected with each face of A and vice versa => 6*4*2 seg_tri intersections
        vector<vector<double>> points;
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 4; j++){

                vector<vector<double>> points1 = intersect_segment_triangle(tetB.edges[i][0].coords, tetB.edges[i][1].coords, tetA.faces[j][0].coords, tetA.faces[j][1].coords, tetA.faces[j][2].coords);

                vector<vector<double>> points2 = intersect_segment_triangle(tetA.edges[i][0].coords, tetA.edges[i][1].coords, tetB.faces[j][0].coords, tetB.faces[j][1].coords, tetB.faces[j][2].coords);
                if(!points1.empty()){
                    for(vector<double> &p : points1){
                        add_vector(points,p);
                    }
                }
                if(!points2.empty()){
                    for(vector<double> &p : points2){
                        add_vector(points,p);
                    }
                }
            }
        }
        return points;
    }

    vector<tetrahedron> construct_intersection(tetrahedron a, tetrahedron b, vector<vector<double>> points){
        // not developed yet
        vector<tetrahedron> tet;

        return tet;
    }

    supermesh::supermesh(node* p0nodes, ele* p0eles, int elcount0, node* p1nodes, ele* p1eles, int elcount1){
        // not developed yet

        //initialize empty vector of tetrahedra
        vector<tetrahedron> result;

        // create tetrahedra of both parents from TETGEN's elements
        vector<tetrahedron> parent0;
        for(int i = 0; i < elcount0; i++){
            tetrahedron temp(p0nodes[p0eles[i].nodes[0]],p0nodes[p0eles[i].nodes[1]],p0nodes[p0eles[i].nodes[2]],p0nodes[p0eles[i].nodes[3]]);
            temp.parents = 0;
            parent0.push_back(temp);
        }
        vector<tetrahedron> parent1;
        for(int i = 0; i < elcount1; i++){
            tetrahedron temp(p1nodes[p1eles[i].nodes[0]],p1nodes[p1eles[i].nodes[1]],p1nodes[p1eles[i].nodes[2]],p1nodes[p1eles[i].nodes[3]]);
            temp.parents = 1;
            parent1.push_back(temp);
        }


        //loop over elements of first parent
        bool* markers = new bool[elcount1];
        for(int i = 0; i < elcount0; i++){


        }
        delete[] markers;
                
    }
//  PRIMARY FUNCTIONS END HERE


