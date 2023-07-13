#include "../source/supermesh.h"
#include "../source/tetrahedron.h"
#include "../source/node.h"
#include "../source/ele.h"
#include <vector>
#include <bitset>
#include <cmath>
#include <unordered_set>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

int seed = 287;

const int var = 2000;

// get random double
double dRand(double fMin, double fMax)
{
    seed += 1092;
    srand((seed*2334+1223)%RAND_MAX);
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// get random point
void pRand(double p[3],int min, int max){
    p[0] = dRand(min,max);
    p[1] = dRand(min,max);
    p[2] = dRand(min,max);
}

bool diff(double p1[3], double p[2], double limit){
    int cnt = 0; 
    for(int i = 0; i < 3; i++){
        if(p[1]==p[2]){
            cnt++;
        }
    }
    if(cnt==3){
        return false;
    }
    return true;
}

// generates a list of tetrahedra in a cube shaped part of the domain defined by min max
void generate_random_tetrahedra(tetrahedron* tetra,int size, int min, int max){
    
    for(int i = 0; i < size; i++){
        double p0[3], p1[3], p2[3], p3[3];
        pRand(p0,min,max);
        pRand(p1,min,max);
        pRand(p2,min,max);
        pRand(p3,min,max);
        while(tet_vol(p0,p1,p2,p3)==0){
            pRand(p0,min,max);
            pRand(p1,min,max);
            pRand(p2,min,max);
            pRand(p3,min,max);
        }
        double arr[12] = {p0[0],p0[1],p0[2],p1[0],p1[1],p1[2],p2[0],p2[1],p2[2],p3[0],p3[1],p3[2]};
        tetrahedron t(arr);
        tetra[i] = t;
    }


}

int main(){

    fflush(stdout);
    double sum = 0;
    tetrahedron tet[var];
    generate_random_tetrahedra(tet,var,-3,12);
    tetrahedron tet2[var];
    generate_random_tetrahedra(tet2,var,1,16);
    tetrahedron tet1[var];
    generate_random_tetrahedra(tet1,var,1,16);



    cout<<"Tests for intersect() function: "<<endl;
    cout<<"\tTesting intersect() on a mix of tetrahedra: \n\t";
    int var_overlap = 0;
    int var_no_overlap = 0;
    for(int i = 0; i < var; i++){
        if(i%100 == 0){
            cout<<"- ";
            fflush(stdout);
        }
        for(int j = 0 ; j < var; j ++){
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            bool intr = intersect(tet[i],tet1[j]);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            if(intr){
                var_overlap++;
            }
            var_no_overlap++;
            sum += time_span.count();
        }
    }
    int both = 2000*2000;
    cout<<"\n\t Checking all intersections takes "<<(sum)<<" ms, with the ratio of overlap/total being: "<<((double)var_overlap/(double)both)<<"\n";

    cout<<"\n\tnow timing the intersect() function for another mix of tetrahedra: "<<endl<<"\t";

    int var_overlap1 = 0;
    int var_no_overlap1 = 0;
    double sum1 = 0;
    for(int i = 0; i < var; i++){
        if(i%100 == 0){
            cout<<"- ";
            fflush(stdout);
        }
        for(int j = 0 ; j < var; j ++){
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            bool intr = intersect(tet1[i],tet2[j]);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            if(intr){
                var_overlap1++;
            }
            var_no_overlap1++;
            sum1 += time_span.count();
        }
    }

    cout<<"\n\t Checking all intersections takes "<<(sum1)<<" ms, with the ratio of overlap/no overlap being: "<<((double)var_overlap1/(double)both)<<"\n";

    double a = var_overlap;
    double b = var_no_overlap;
    double c = var_overlap1;
    double d = var_no_overlap1;
    double k = 1/(a*d - b*c);
    double x = d*k*sum - b*k*sum1;
    double y = a*k*sum1 - c*k*sum;
    cout<<"\tOn average, it took "<<x<<" ms to detect an overlap and "<<y<<" ms to detect no overlap.\n";
    

    cout<<"\n\nTests for get_intersection_points() function: "<<endl<<"\t";
    cout<<"Testing get_intersection_points() on mix of tetrahedra: \n\t";
    sum = 0;
    int var_overlap2 = 0;
    int var_no_overlap2 = 0;
    int var_overlap3 = 0;
    int var_no_overlap3 = 0;
    for(int i = 0; i < var ; i++){
        if(i%100 == 0){
            cout<<"- ";
             fflush(stdout);
        }
        for(int j = 0 ; j < var; j ++){
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            vector<vector<double>> v = get_intersection_points(tet[i],tet1[j]);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            if(v.size()){
                var_overlap2++;
            }
            var_no_overlap2++;
            sum += time_span.count();
        }
    }
    cout<<"\n\t Checking all intersections takes "<<(sum)<<" ms\n";

    cout<<"\tTesting get_intersection_points() on another mix of tetrahedra: \n\t";
    sum1 = 0;
    int counter[16] = {0};
    for(int i = 0; i < var ; i++){
        if(i%100 == 0){
            cout<<"- ";
             fflush(stdout);
        }
        for(int j = 0 ; j < var; j ++){
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            vector<vector<double>> v = get_intersection_points(tet1[i],tet2[j]);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            counter[v.size()]++;
            if(v.size()){
                var_overlap3++;
            }
            var_no_overlap3++;
            sum1 += time_span.count();
        }
    }
    cout<<"\n\t Checking all intersections takes "<<(sum1)<<" ms\n\t";
    a = var_overlap2;
    b = var_no_overlap2;
    c = var_overlap3;
    d = var_no_overlap3;
    k = 1/(a*d - b*c);
    x = d*k*sum - b*k*sum1;
    y = a*k*sum1 - c*k*sum;
    cout<<"\tOn average, it took "<<x<<" ms to detect an overlap and "<<y<<" ms to detect no overlap.\n\n";

    cout<<"Ratio of no intersection for first set (intersect()/get_intersections_points()): "<<(double)var_no_overlap/(double)var_no_overlap2<<endl;
    cout<<"Ratio of intersection for second set (intersect()/get_intersections_points()): "<<(double)var_no_overlap1/(double)var_no_overlap3<<endl;
    cout<<"\tBelow is the distribution of the lengths of the obtained vectors: \n\t";
    for(int i = 0; i < 16; i ++){
        cout<<i<<":"<<counter[i]<<"| ";
    }
    cout<<endl;
}