#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <myLib\myLib.h>

using namespace std;

double Diff = 0.02;
const double OceanLength = 5;
const double OceanWidth = 4;
const long double pi = 3.1415926535897932384626;
const long double BeamAngel = 0.5 * pi;

#define ll long long
#define ld double

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

int GetNext_Ship(ld deepth,int idx)
{
    if(idx > OceanWidth / Diff - 1)
        return OceanWidth / Diff - 1;
    ld length = deepth*tan(Deg2Rad(60));
    return idx + length2idx(length)/*  + (length2idx(length) == idx) */;
}

int GetNext_SoundingAreaEdge(int idx,vector<ld> &SoundingAreaEdge)
{
    if(idx > SoundingAreaEdge.size() - 1)
        return SoundingAreaEdge.size() - 1;
    // ld length = 0;
    // for(;idx2length(idx) + length < OceanWidth;length += Diff)
    //     if(length * tan(Deg2Rad(60)) > SoundingAreaEdge[idx+length2idx(length)])
    //     {
    //         int tmp = idx + length2idx(length);
    //         return tmp/*  + (length2idx(length) == idx) */;
    //     }
    // return SoundingAreaEdge.size() - 1;
    ll addLengthIdx = 0;
    for(;idx + addLengthIdx < SoundingAreaEdge.size();addLengthIdx++)
        if(SoundingAreaEdge[idx + addLengthIdx] < addLengthIdx * Diff * tan(Deg2Rad(30)))
            return idx + addLengthIdx;
    return SoundingAreaEdge.size() - 1;
}

int main()
{
    //open input and output file
    freopen("src\\cacu.in", "r", stdin);

    //apply and initialize variables
    ll SubdivNum_L_I = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W_I = (ll)ceil(OceanWidth / Diff) +1;
    //ll SubdivNum = SubdivNum_L * SubdivNum_W;
    vector<vector<ld>> OceanDeepth_Init(SubdivNum_L_I+10, vector<ld>(SubdivNum_W_I+10, 0));

    //input ocean deepth
    for(int i = 0;i < SubdivNum_L_I;i++)
        for(int j = 0;j < SubdivNum_W_I;j++)
            cin >> OceanDeepth_Init[i][j];

    //convert deepth to Nm
    for(int i = 0;i < SubdivNum_L_I;i ++)
        for(int j = 0;j < SubdivNum_W_I;j++)
            OceanDeepth_Init[i][j] = m2Nm(OceanDeepth_Init[i][j]);

    Diff = 0.005;
    ll SubdivNum_L = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W = (ll)ceil(OceanWidth / Diff) +1;
    vector<vector<ld>> OceanDeepth_T(SubdivNum_L_I+10, vector<ld>(SubdivNum_W+10, 0));
    
    vector<ld> SoundingAreaEdge(SubdivNum_L+10, 0);
    vector<int> ShipLocation(SubdivNum_L+10,0);

    //interpolate the ocean deepth
    for(int i = 0;i < SubdivNum_L_I;i++)
        for(int j = 0;j < SubdivNum_W_I;j++)
        {
            int idxl = 4*j,
                idxr = 4*j+4;
            ld div;
            if(j < SubdivNum_W_I-1)div = OceanDeepth_Init[i][j+1] - OceanDeepth_Init[i][j];
            else div = OceanDeepth_Init[i][j] - OceanDeepth_Init[i][j-1];
            for(int k = 0;k < 4;k++)
                OceanDeepth_T[i][idxl+k] = OceanDeepth_Init[i][j] + div * k / 4;
        }

    OceanDeepth_Init.clear();

    vector<vector<ld>> OceanDeepth(SubdivNum_L+10, vector<ld>(SubdivNum_W+10, 0));
    
        
    for(int i = 0;i < SubdivNum_W;i ++)
        for(int j = 0;j < SubdivNum_L_I;j++)
        {
            int idxu = 4*j,
                idxd = 4*j+4;
            ld div;
            if(j < SubdivNum_L_I-1) div = OceanDeepth_T[j+1][i] - OceanDeepth_T[j][i];
            else div = OceanDeepth_T[j][i] - OceanDeepth_T[j-1][i];
            for(int k = 0;k < 4;k++)
                OceanDeepth[idxu+k][i] = OceanDeepth_T[j][i] + div * k / 4; 
        }
    

    OceanDeepth_Init.clear();

    //initialize the first sounding area edge
    for(int i = 0;i < SubdivNum_L;i++)
        SoundingAreaEdge[i] = GetNext_SoundingAreaEdge(0,OceanDeepth[i]);

    //caculate the sounding area edge
    vector<int> SoundingEdgeList;
    SoundingEdgeList.push_back(0);

    freopen("src\\cacu.out", "w", stdout);
    for(;;)
    {
        // UpdatePrc(SubdivNum_L,SoundingEdgeList[SoundingEdgeList.size()-1],40,"Caculating Sounding Area Edge");
        int minShipLocation = SubdivNum_W;
        bool outputflag = false;
        if(ShipLocation[0] == 126)
            outputflag = true;
        int minSoundingAreaEdge = INT_MAX;

        if(outputflag)
            cout << "SoundingEdgeList: ";
        for(int i = 0;i < SoundingEdgeList.size();i++)
            if(outputflag)
                cout << SoundingEdgeList[i] << " ";
        if(outputflag) cout << endl;

        if(outputflag)
            cout << "SoundingAreaEdge: ";
        for(int i = 0;i < SubdivNum_L;i++)
            if(outputflag)
                cout << SoundingAreaEdge[i] << " ";
        if(outputflag) cout << endl;

        for(int i = 0;i < SubdivNum_L;i++)
            ShipLocation[i] = GetNext_Ship(OceanDeepth[i][SoundingAreaEdge[i]],SoundingAreaEdge[i]);
        for(int i = 0;i < SubdivNum_L;i++)
            if(ShipLocation[i] < minShipLocation)
                minShipLocation = ShipLocation[i];

        if(outputflag)
            cout << "ShipLocation: ";
        for (int i = 0; i < SubdivNum_L; i++)
            if(outputflag)
                cout << ShipLocation[i] << " ";
        if(outputflag) cout << endl;

        for(int i = 0;i < SubdivNum_L;i++)
            ShipLocation[i] = minShipLocation;

        SoundingEdgeList.push_back(minShipLocation);

        for(int i = 0;i < SubdivNum_L;i++)
            SoundingAreaEdge[i] = GetNext_SoundingAreaEdge(minShipLocation,OceanDeepth[i]);

        for(int i = 0;i < SubdivNum_L;i++)
            if(SoundingAreaEdge[i] < minSoundingAreaEdge)
                minSoundingAreaEdge = SoundingAreaEdge[i];

        if(minShipLocation >= SubdivNum_W-1 || minSoundingAreaEdge >= SubdivNum_W-1)
            break;
    }

    for(int i = 0;i < SoundingEdgeList.size();i++)
        cout << SoundingEdgeList[i] << " ";

    return 0;
}
