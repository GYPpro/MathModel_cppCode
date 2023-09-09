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

ld Rad2Deg(ld rad) // radian to degree
{
    return rad * 180 / pi;
}

ld Deg2Rad(ld deg) // degree to radian
{
    return deg * pi / 180;
}

ld Nm2m(ld nm) // nautical mile to meter
{
    return nm * 1852;
}

ld m2Nm(ld m) // meter to nautical mile
{
    return m / 1852;
}

int length2idx(ld length) // length to index
{
    return (int)ceil(length / Diff);
}

ld idx2length(int idx) // index to length
{
    return idx * Diff;
}

int GetNextEdge(ld deepth,int idx)
{
    ld length = deepth*cos(BeamAngel/2);
    return idx + length2idx(length);
}

int main()
{
    //open input and output file
    freopen("src\\cacu.in", "r", stdin);
    freopen("src\\cacu.out", "w", stdout);

    //apply and initialize variables
    ll SubdivNum_L = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W = (ll)ceil(OceanWidth / Diff) +1;
    ll SubdivNum = SubdivNum_L * SubdivNum_W;
    vector<vector<ld>> OceanDeepth(SubdivNum_L, vector<ld>(SubdivNum_W, 0));
    vector<ld> SoundingAreaEdge(SubdivNum_L, 0);
    vector<int> ShipLocation(SubdivNum_L,0);

    //input ocean deepth
    for(int i = 0;i < SubdivNum_L;i++)
        for(int j = 0;j < SubdivNum_W;j++)
            cin >> OceanDeepth[i][j];

    //initialize the first sounding area edge
    for(int i = 0;i < SubdivNum_L;i++)
        SoundingAreaEdge[i] = OceanDeepth[i][0];

    //caculate the sounding area edge
    vector<int> SoundingEdgeList;
    for(;;)
    {
        int minIdx = 0;
        for(int i = 0;i < SubdivNum_L;i++)
            if(SoundingAreaEdge[i] < SoundingAreaEdge[minIdx])
                if(minIdx < SubdivNum_L - 1)
                    minIdx = i;
    }
}
