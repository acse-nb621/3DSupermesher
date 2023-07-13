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


bool equals(vector<double> v1, vector<double> v2){
    for(int i = 0; i < 3; i++){
        if (abs(v1[i]-v2[i])>pow(10,-4)){
            return false;
        }
    }
    return true;
}

bool test_get_intersection_points(double A[12], double B[12], vector<vector<double>> points){
    tetrahedron tetA(A);
    tetrahedron tetB(B);
    vector<vector<double>> int_points =  get_intersection_points(tetA,tetB);
    if(points.empty() && int_points.empty()){
        return true;
    }

    if(int_points.size() != points.size()){
        return false;
    }

    vector<bool> checked(points.size(),false);
    for(int i = 0; i < int_points.size(); i++){
        bool found = false;
        for(int j = 0; j < points.size() && !found; j++){
            if(checked[j]){
                continue;
            }
            if(equals(points[j],int_points[i])){
                checked[j] = 1;
                found = true;
            }
        }
        if(!found){
            return false;
        }
    }
    return true;
}




int main(){
    bool passed;
    vector<vector<double>> intersection;
    // test 1
    // 1st tetrahedron 0,0,0 1,1,0 2,0,0 1,0,1
    // 2nd tetrahedron 2,0,-0.5 2.5,0.5,0 4,0,0 3,0,1
    // intersection: {} , should return empty vector
    double tetA[12] = {0,0,0,1,1,0,2,0,0,1,0,1};
    double tetB[12] = {2,0,-0.5,2.5,0.5,0,4,0,0,3,0,1};
    bool passed1 = test_get_intersection_points(tetA,tetB,intersection);
    cout<<passed1<<endl;


    // 1st tetrahedron 0,0,0 1,1,0 2,0,0 1,0,1
    // 2nd tetrahedron 2,1,1 2,1,0 3,-1,0 1,0.5,0.75
    // intersection: {} , should return empty vector

    double tetC[12] = {0,0,0,1,1,0,2,0,0,1,0,1};
    double tetD[12] = {2,1,1,2,1,0,3,-1,0,1,0.5,0.75};
     
    bool passed2 = test_get_intersection_points(tetC,tetD,intersection);
    cout<<passed2<<endl;

    // 1st tetrahedron 0,0,0 1,1,0 2,0,0 1,0,1
    // 2nd tetrahedron 0.5,0,-0.5 2.5,0.5,0 4,0,0 3,0,1
    // intersection: {(1.75,0,0.25),(1.8,0.2,0),(1.33,0,0),(2,0,0)}
    double tetE[12] = {0,0,0,1,1,0,2,0,0,1,0,1};
    double tetF[12] = {0.5,0,-0.5, 2.5,0.5,0,4,0,0,3,0,1};
    vector<vector<double>> intersection1{{1.75,0,0.25},{1.8,0.2,0},{1.33333,0,0},{2,0,0}};
    bool passed3 = test_get_intersection_points(tetE,tetF,intersection1);
    cout<<passed3<<endl;

    // 1st tetrahedron 0,0,0 1,1,0 2,0,0 1,0,1
    // 2nd tetrahedron 0.5,0,-0.5 2.5,0.5,0 4,0,0 1,0,0.75
    // intersection: {(1.75,0,0.25),(1.8,0.2,0),(1.33,0,0)}

    double tetG[12] = {0,0,0,1,1,0,2,0,0,1,0,1};
    double tetH[12] = {0.5,0,-0.5,2.5,0.5,0,4,-1,0,1,0.5,0.75};
    vector<vector<double>> intersectionGH{{1.25,0,0},{0.7,0.2,0},{0.9,0.4,0.5},{1.64286,0.357143,0},{1,0.411765,0.588235},{1,0.4375,0.5625},{1.7,0,0.3}};
    bool passed4 = test_get_intersection_points(tetG,tetH,intersectionGH);

    cout<<passed4<<endl;

    // 1st tetrahedron 0,0,0 1,1,0 2,0,0 1,0,1
    // 2nd tetrahedron 2,1,1 2,1,0 3,-1,1 0.5,0.5,0.5
    // intersection: {(1.333,0,0.67),(1,0.38,0.63),(1,0.67,0.33)}

}