
using namespace std;

double Diff = 0.02;                                       //分度值
const double OceanLength = 5;                             //海域长度
const double OceanWidth = 4;                              //海域宽度
const long double pi = 3.1415926535897932384626;          //圆周率
const long double BeamAngel = 0.66666666666666666 * pi;   //波束角 

#define ll long long
#define ld double


ld Rad2Deg(ld rad);// 弧度制转角度制

ld Deg2Rad(ld deg); // 角度制转弧度制

ld Nm2m(ld nm); // 海里转米

ld m2Nm(ld m); // 米转海里

int length2idx(ld length); // 从以海里为单位的连续长度转换为离散的、基于Diff的坐标

ld idx2length(int idx); // 同上，从坐标转为海里

ld Pyt(ld x1,ld y1,ld x2,ld y2); // 求两点间距离

int GetNext_Ship(ld deepth,int idx); // 基于测线左端点的深度和测线左端点的坐标，计算船的坐标

int GetNext_SoundingAreaEdge(int idx,vector<ld> &SoundingEdgeArea); // 基于船的坐标，计算测线右端点的坐标

int GetLast_SoundingAreaEdge(int idx,vector<ld> &SoundingEdgeArea); // 基于船的坐标，计算测线左端点的坐标

// Path: code/cacu.cpp
