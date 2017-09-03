#include "main.h"
#include "distance.h"

const int maxDist = 9999;
const vector<string> targetNode = {"C13", "C15", "C16", "C18"};
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
//        else
//            cout<<i->m_node1<<"\t"<<i->m_node2<<"\t"<<i->m_ifAvailable<<endl;

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
    for(auto i = nodeVec.begin(); i!= nodeVec.end(); i++)
    {
        i->m_minDistance.resize(targetNode.size());
        i->m_route.resize(targetNode.size());
    }
    for( int k = 0; k< targetNode.size(); k++)
    {
        int target = dict[targetNode[k]];
        vector<int> dist(nodeVec.size());
        vector<int> preN(nodeVec.size());           //记录最短路径前驱节点
        vector<bool> s(nodeVec.size());             //判断节点是否加入s集合
        for(int i=0; i<nodeVec.size(); i++)
        {
            nodeVec[i].m_route[k].clear();
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
            nodeVec[i].m_minDistance[k] = dist[i];
            int temp = i;
            while(temp != target)
            {
                nodeVec[i].m_route[k].push_back(revDict[temp]);
                temp = preN[temp];
            }
        }
    }
}

void ChooseCargonRoute(vector<CNode> &nodeVec, const vector<CLink> &linkVec)
{
    BrushMinDistance(nodeVec, linkVec);         //刷新最短路径
    for(int i =0;i<nodeVec.size();i++)
    {
        vector<double> cost(4,0);                        //计算四种路线的距离
        for(int j=0;j<cost.size();j++)
        {
            int dis = nodeVec[i].m_minDistance[j]+nodeVec[dict[re_pointCityMap[j]]].m_minDistance[3];
            cost[j] = dis;
            if(j!=3)
                cost[j] /= 2;
        }
        auto it = min_element(cost.begin(), cost.end());
        int position = distance(cost.begin(), it);
        if(position != cost.size()-1)
        {
            nodeVec[i].m_cargonDistance   = *it * 2;
            nodeVec[i].m_ifTransfer = true;
        }
        else
        {
            nodeVec[i].m_cargonDistance   = *it;
            nodeVec[i].m_ifTransfer = false;
        }


        nodeVec[i].m_cargonRoute.insert(nodeVec[i].m_cargonRoute.end(),
                                        nodeVec[i].m_route[position].begin(), nodeVec[i].m_route[position].end());
        //找到position对应的城市编号
        auto it2 = find_if(pointCityMap.begin(), pointCityMap.end(),
                           [position](const pair<string, int> &s) ->bool{ return s.second == position;});
        nodeVec[i].m_cargonRoute.insert(nodeVec[i].m_cargonRoute.end(),
                                        nodeVec[dict[it2->first]].m_route[3].begin(),
                                        nodeVec[dict[it2->first]].m_route[3].end());
        nodeVec[i].m_cargonRoute.push_back("C18");
    }

}


map<string, int> pointCityMap= {
    {"C13", 0},{"C15", 1},{"C16", 2},{"C18", 3}
};
map<int, string> re_pointCityMap ={
    {0, "C13"},{1, "C15"},{2, "C16"},{3, "C18"}
};
void PrintRoute(const vector<CNode> &nodeVec, const string &target)
{
    for(auto i=nodeVec.begin(); i!= nodeVec.end(); i++)
    {
        cout<< i->m_label <<" 的最短距离为： "<< i->m_minDistance[pointCityMap[target]] <<endl;
        cout<< i->m_label << " 的最短路径经过节点为:\t";
        vector<string> temp = (i->m_route[pointCityMap[target]]);
        for(auto j=temp.begin(); j!=temp.end(); j++)
            cout<<*j<<"\t";
        cout<< targetNode[pointCityMap[target]] <<endl;
    }
}
