#include "../source/supermesh.h"
#include "../source/tetrahedron.h"
#include "../source/node.h"
#include "../source/ele.h"
#include <vector>
#include <bitset>
#include <cmath>
#include <unordered_set>
#include <algorithm>

// g++ ../node.cpp ../face.cpp ../ele.cpp ../supermesh.cpp get_intersection_points.cpp -o test
// ./test

using namespace std;

bool test_intersect(double A[12], double B[12], bool flag){
    if(intersect(A,B) == flag){
        return true;
    }
    return false;
}

int main(){

    // share a plane: intersection
    double A[12] = {0,0,0,2,0,2,4,0,0,2,-2,0};
    double B[12] = {3,0,1.5,2,0,-1,5,-1,0,4,3,-1};
    bool passed1 = test_intersect(A,B,true);
    cout<<passed1<<endl;

    // share a line: intersection
    double C[12]= {0,0,0,2,0,2,4,0,0,2,-2,0};
    double D[12] = {3,0,-1.5,1,0,0,5,0,0,4,3,-1};
    bool passed2 = test_intersect(C,D,true);
    cout<<passed2<<endl;

    // do not intersect
    double E[12] = {0,0,0,2,0,2,4,0,0,2,-2,0};
    double F[12] = {3,0,-1.5,1,0,-1,5,-1,0,4,3,-1};
    bool passed3 = test_intersect(E,F, false);
    cout<<passed3<<endl;
    

    // intersection in 3D region
    double G[12] = {0,0,0,2,0,2,4,0,0,2,-2,0};
    double H[12] = {3,0,-1.5,1,0,-1,5,-1,3,4,3,-1};
    bool passed4 = test_intersect(G,H,true);
    cout<<passed4<<endl;




}
