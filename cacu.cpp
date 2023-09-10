#pragma GCC optimize(2)

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "cacu.h"

using namespace std;

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

int GetNext_SoundingAreaEdge(int idx,vector<ld> &SoundingEdgeArea)// 基于船的坐标，计算测线右端点的坐标
{
    if(idx > SoundingEdgeArea.size() - 1)
        return SoundingEdgeArea.size() - 1;

    ll addLengthIdx = 0;
    // 以DIFF为单位递增Length，直到Length对应的测线与海底相交
    for(;idx + addLengthIdx < SoundingEdgeArea.size();addLengthIdx++)
        if(SoundingEdgeArea[idx + addLengthIdx] < addLengthIdx * Diff * tan(Deg2Rad(30)))
            return idx + addLengthIdx;
    return SoundingEdgeArea.size() - 1;
}

int GetLast_SoundingAreaEdge(int idx,vector<ld> &SoundingEdgeArea)// 基于船的坐标，计算测线左端点的坐标
{
    if(idx < 0)
        return 0;

    ll addLengthIdx = 0;
    // 以DIFF为单位递增Length，直到Length对应的测线与海底相交
    for(;idx - addLengthIdx >= 0;addLengthIdx++)
        if(SoundingEdgeArea[idx - addLengthIdx] < addLengthIdx * Diff * tan(Deg2Rad(30)))
            return idx - addLengthIdx;
    return 0;
}

ld GetW(int l,int r,vector<ld> &OceanDeepth)//求用于计算重叠率的海床长度W
{
    ld W = 0;
    for(int i = l;i < r;i++)
        W += Pyt(idx2length(i),OceanDeepth[i],idx2length(i+1),OceanDeepth[i+1]);
    return W;
}

int main()
{
    //打开文件
    freopen("cacu.in", "r", stdin);

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
    
    vector<ld> SoundingEdgeArea(SubdivNum_L+10, 0);
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

    freopen("cacu.out", "w", stdout);
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
            cout << "SoundingEdgeArea: ";
        for(int i = 0;i < SubdivNum_L;i++)
            if(outputflag)
                cout << SoundingEdgeArea[i] << " ";
        if(outputflag) cout << endl;

        for(int i = 0;i < SubdivNum_L;i++)
            ShipLocation[i] = GetNext_Ship(OceanDeepth[i][SoundingEdgeArea[i]],SoundingEdgeArea[i]);
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
            SoundingEdgeArea[i] = GetNext_SoundingAreaEdge(minShipLocation,OceanDeepth[i]);

        for(int i = 0;i < SubdivNum_L;i++)
            if(SoundingEdgeArea[i] < minSoundingAreaEdge)
                minSoundingAreaEdge = SoundingEdgeArea[i];

        if(minShipLocation >= SubdivNum_W-1 || minSoundingAreaEdge >= SubdivNum_W-1)
            break;
    }

    //输出总长度
    cout << "Total Length: " << SoundingEdgeList.size()*OceanLength << endl;

    //输出每条测线的坐标
    cout << "SoundingEdgeList: ";
    for(int i = 0;i < SoundingEdgeList.size();i++)
        cout << idx2length(SoundingEdgeList[i]) << " ";

    //对每条测线统计重叠率并计算重叠率大于20的长度
    ld OverlapLength = 0;
    for(int i = 0;i < SoundingEdgeList.size()-1;i ++)
    {
        for(int j = 0;j < SubdivNum_L;j ++)
        {
            int idxl = GetLast_SoundingAreaEdge(SoundingEdgeList[i],OceanDeepth[j]),
                idxr = GetNext_SoundingAreaEdge(SoundingEdgeList[i],OceanDeepth[j]);
            ld W = idx2length(SoundingEdgeList[i+1]-SoundingEdgeList[i]);
            if(idxl <= idxr)
                W = GetW(idxl,idxr,OceanDeepth[j]);
            if(idx2length(SoundingEdgeList[i+1]-SoundingEdgeList[i])/W < 0.8) OverlapLength += 1;
            // else 
            //     cout << "debug";
        }
    }
    cout << endl << "OverlapLength: " << OverlapLength * Diff << endl;

    return 0;
}
