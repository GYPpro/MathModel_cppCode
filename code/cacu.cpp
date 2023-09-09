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
const long double BeamAngel = 0.66666666666666666 * pi;

#define ll long long
#define ld double

ld Rad2Deg(ld rad) // 弧度制转角度制
{
    return rad * 180 / pi;
}

ld Deg2Rad(ld deg) // 角度制转弧度制
{
    return deg * pi / 180;
}

ld Nm2m(ld nm) // 海里转米
{
    return nm * 1852;
}

ld m2Nm(ld m) // 米转海里
{
    return m / 1852;
}

int length2idx(ld length) // 从以海里为单位的连续长度转换为离散的、基于Diff的坐标
{
    return (int)ceil(length / Diff);
}

ld idx2length(int idx) // 同上，从坐标转为海里
{
    return idx * Diff;
}

ld Pyt(ld x1,ld y1,ld x2,ld y2)// 求两点间距离
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int GetNext_Ship(ld deepth,int idx)// 基于测线左端点的深度和测线左端点的坐标，计算船的坐标
{
    if(idx > OceanWidth / Diff - 1)
        return OceanWidth / Diff - 1;
    ld length = deepth*tan(Deg2Rad(60));
    return idx + length2idx(length);
}

int GetNext_SoundingAreaEdge(int idx,vector<ld> &SoundingAreaEdge)// 基于船的坐标，计算测线右端点的坐标
{
    if(idx > SoundingAreaEdge.size() - 1)
        return SoundingAreaEdge.size() - 1;

    ll addLengthIdx = 0;
    // 以DIFF为单位递增Length，直到Length对应的测线与海底相交
    for(;idx + addLengthIdx < SoundingAreaEdge.size();addLengthIdx++)
        if(SoundingAreaEdge[idx + addLengthIdx] < addLengthIdx * Diff * tan(Deg2Rad(30)))
            return idx + addLengthIdx;
    return SoundingAreaEdge.size() - 1;
}

int GetLast_SoundingAreaEdge(int idx,vector<ld> &SoundingAreaEdge)// 基于船的坐标，计算测线左端点的坐标
{
    if(idx < 0)
        return 0;

    ll addLengthIdx = 0;
    // 以DIFF为单位递增Length，直到Length对应的测线与海底相交
    for(;idx - addLengthIdx >= 0;addLengthIdx++)
        if(SoundingAreaEdge[idx - addLengthIdx] < addLengthIdx * Diff * tan(Deg2Rad(30)))
            return idx - addLengthIdx;
    return 0;
}

ld GetW(int r,int l,vector<ld> &OceanDeepth)//求用于计算重叠率的海床长度W
{
    ld W = 0;
    for(int i = l;i < r;i++)
        W += Pyt(idx2length(i),OceanDeepth[i],idx2length(i+1),OceanDeepth[i+1]);
    return W;
}

int main()
{
    //打开文件
    freopen("src\\cacu.in", "r", stdin);

    //申请并初始化原始海底深度矩阵
    ll SubdivNum_L_I = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W_I = (ll)ceil(OceanWidth / Diff) +1;
    vector<vector<ld>> OceanDeepth_Init(SubdivNum_L_I+10, vector<ld>(SubdivNum_W_I+10, 0));

    //输入原始海底深度
    for(int i = 0;i < SubdivNum_L_I;i++)
        for(int j = 0;j < SubdivNum_W_I;j++)
            cin >> OceanDeepth_Init[i][j];

    //将原始海底深度转换为以海里为单位
    for(int i = 0;i < SubdivNum_L_I;i ++)
        for(int j = 0;j < SubdivNum_W_I;j++)
            OceanDeepth_Init[i][j] = m2Nm(OceanDeepth_Init[i][j]);

    Diff = 0.005; // 重设Diff为0.005，以确保计算准确
    ll SubdivNum_L = (ll)ceil(OceanLength / Diff) +1;
    ll SubdivNum_W = (ll)ceil(OceanWidth / Diff) +1;
    vector<vector<ld>> OceanDeepth_T(SubdivNum_L_I+10, vector<ld>(SubdivNum_W+10, 0));
    
    vector<ld> SoundingAreaEdge(SubdivNum_L+10, 0);
    vector<int> ShipLocation(SubdivNum_L+10,0);

    //对原海底深度数据做插值，得到以Diff为单位的海底深度矩阵，并销毁多余数据
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

    //申请记录每条测线的数组
    vector<int> SoundingEdgeList;

    freopen("src\\cacu.out", "w", stdout);
    //递推，计算每条测线
    for(;;)
    {
        int minShipLocation = SubdivNum_W;
        bool outputflag = false;
        if(ShipLocation[0] == 131)
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
