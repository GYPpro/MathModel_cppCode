#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

const double Diff = 0.02;
const double OceanLength = 5;
const double OceanWidth = 4;
const long double pi = 3.1415926535897932384626;
const long double BeamAngel = 0.5 * pi;

#define ll long long
#define ld long double

int main()
{
    freopen("src\\cacu.in", "r", stdin);
    freopen("src\\cacu.out", "w", stdout);
    ll SubdivNum_L = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W = (ll)ceil(OceanWidth / Diff) +1;
    ll SubdivNum = SubdivNum_L * SubdivNum_W;
    vector<vector<ld>> OceanDeepth(SubdivNum_L, vector<ld>(SubdivNum_W, 0));
    for(int i = 0;i < SubdivNum_L;i++)
        for(int j = 0;j < SubdivNum_W;j++)
            cin >> OceanDeepth[i][j];
    
}
