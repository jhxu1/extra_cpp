#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <deque>
using namespace std;
class CNode
{
public:
    const std::string m_label;          //节点标号
    const float m_cargoNum;             //每年货物数量（千）
    vector<size_t> m_minDistance;               //最短路径长度
    vector<vector<string> > m_route;             //最短路径经过的节点
    size_t m_cargonDistance;                //运货路径长度
    vector<string> m_cargonRoute;           //运货路径
    double m_logistCost;                //该城市到C15的物流成本
    bool m_ifTransfer;                  //是否中转

    CNode() = default;
    CNode(const std::string &label, float cargoNum)
        :m_label(label), m_cargoNum(cargoNum), m_logistCost(0), m_ifTransfer(false){};

};

class CLink
{
public:
    const string m_node1;               //节点1
    const string m_node2;               //节点2
    const size_t m_distance;            //链接长度
    bool m_ifAvailable;                 //是否已经建造
    float m_builtCost;                    //建造成本

    CLink() = default;
    CLink(string node1, string node2, size_t weight, bool ifAvailable, float builtCost)
        :m_node1(node1), m_node2(node2), m_distance(weight), m_ifAvailable(ifAvailable), m_builtCost(builtCost){};
};

extern const size_t g_allDist;          //总路程数
#endif // MAIN_H_INCLUDED
