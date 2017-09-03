#include "main.h"

vector<vector<int>> createMatrix(const vector<CNode> &nodeVec, const vector<CLink> &linkVec);       //产生距离矩阵
void PrintDistance(const vector<vector<int>> &distance);                                            //输出距离矩阵
void BrushMinDistance(vector<CNode> &nodeVec, const vector<CLink> &linkVec);                        //刷新每个点到C15的最短距离
void PrintRoute(const vector<CNode> &nodeVec);                                                      //输出每个点到C15最短距离经过的点

extern map<string, int> dict;
extern map<int, string> revDict;
