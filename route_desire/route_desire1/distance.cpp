#include "main.h"
#include "distance.h"

const int maxDist = 9999;
const string targetNode = "C15";
using namespace std;

map<string, int> dict;
map<int, string> revDict;
/*
生成字典
*/
void createDict(const vector<CNode> &nodeVec)
{
    int sum=0;
    for(auto it = nodeVec.begin();it != nodeVec.end(); it++)
    {
        dict.insert( pair<string, int>(it->m_label, sum) );
        revDict.insert(pair<int, string>(sum, it->m_label));
        sum++;
    }
}
/*
生成距离矩阵
*/
vector<vector<int>> createMatrix(const vector<CNode> &nodeVec, const vector<CLink> &linkVec)
{
    createDict(nodeVec);
    vector<vector<int>> distance(nodeVec.size());
    for(int i=0;i<distance.size();i++)
    {
        distance[i].resize(linkVec.size());
        for(int j=0; j<linkVec.size(); j++)
        {
            if(i != j)
                distance[i][j] = maxDist;
            else
                distance[i][j] = 0;
        }
    }
    for(auto i = linkVec.begin();i != linkVec.end(); i++)
    {
        if(i->m_ifAvailable == true)
        {
            int node1 = dict[i->m_node1];
            int node2 = dict[i->m_node2];
            distance[node1][node2] = i->m_distance;
            distance[node2][node1] = i->m_distance;
        }

    }
    return distance;
}

/*
输出距离矩阵
*/
void PrintDistance(const vector<vector<int>> &distance)
{
    ofstream outfile("最短距离矩阵.csv");
    for(int i=0;i<distance.size();i++)
    {
        for(int j=0;j<distance.size();j++)
        {
            cout<<distance[i][j]<<" ";
            outfile<<distance[i][j]<<",";
        }
        cout<<endl;
        outfile<<endl;
    }
}

/*
刷新最短路径
*/
void BrushMinDistance( vector<CNode> &nodeVec, const vector<CLink> &linkVec)
{
    vector<vector<int>> distance = createMatrix(nodeVec, linkVec);             //产生距离矩阵
    int target = dict[targetNode];
    vector<int> dist(nodeVec.size());
    vector<int> preN(nodeVec.size());           //记录最短路径前驱节点
    vector<bool> s(nodeVec.size());             //判断节点是否加入s集合
    for(int i=0; i<nodeVec.size(); i++)
    {
        nodeVec[i].m_route.clear();
        dist[i] = distance[i][target];
        s[i] = false;
        if(dist[i] == maxDist)
            preN[i] = -1;
        else
            preN[i] = target;
    }
    //加入第一个点target
    dist[target] = 0;
    s[target] = true;
    for(int i=0;i<nodeVec.size();i++)
    {
        int minDist = maxDist;
        int u = target;
        for(int j=0;j<nodeVec.size();j++)
        {
            if(!s[j] && dist[j] < minDist)
            {
                u = j;
                minDist = dist[j];
            }
        }
        s[u] = true;
        for(int j=0;j<nodeVec.size();j++)
        {
            if(!s[j] && dist[u] + distance[j][u] <dist[j])
            {
                dist[j] = dist[u] + distance[j][u];
                preN[j] = u;
            }
        }
    }
    for(int i=0;i<nodeVec.size();i++)
    {
        nodeVec[i].m_minDistance = dist[i];
        int temp = i;
        while(temp != dict["C15"])
        {
            nodeVec[i].m_route.push_back(revDict[temp]);
            temp = preN[temp];
        }
    }
}

void PrintRoute(const vector<CNode> &nodeVec)
{
    for(auto i=nodeVec.begin(); i!= nodeVec.end(); i++)
    {
        cout<< i->m_label <<" 的最短距离为： "<< i->m_minDistance <<endl;
        cout<< i->m_label << " 的最短路径经过节点为:\t";
        vector<string> temp = (i->m_route);
        for(auto j=temp.begin(); j!=temp.end(); j++)
            cout<<*j<<"\t";
        cout<< targetNode <<endl;
    }
}
