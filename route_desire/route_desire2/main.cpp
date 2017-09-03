#include "main.h"
#include "distance.h"
#include "greedy.h"

//全局变量
const size_t g_allDist = 2000;
//--------
using namespace std;

void InputData(vector<CNode> &nodeVec, vector<CLink> &linkVec)
{
    ifstream infile("年均出货批次.txt");
    string str;float in;
    while(infile >> str >> in)
        nodeVec.push_back(CNode(str,in));
    infile.close();
    infile.open("城市之间的线路里程.txt");
    string node1, node2;
    size_t distance;
    bool ifAvailabel;
    float builtCost;
    while(infile >> node1 >> node2 >> distance >> ifAvailabel >> builtCost)
        linkVec.push_back(CLink(node1, node2, distance, ifAvailabel, builtCost));

}
void PrintCost(const vector<CNode> &nodeVec)
{
    size_t sum = 0;
    for(auto it = nodeVec.begin(); it!= nodeVec.end(); it++)
    {
        cout << it->m_label << " 的总物流费用为： " << it->m_logistCost <<endl;
        sum += it->m_logistCost;
    }
    cout << "总物流费用为： " << sum << endl;
}


void PrintPath(vector<CNode> &nodeVec)
{
    ofstream outfile("路径.csv");
    outfile<<"城市编号"<<","<<"金额（千万）"<<","<<"路径"<<endl;
    for(auto i= nodeVec.begin(); i!= nodeVec.end(); i++)
    {
        if(i->m_ifTransfer == true)
            i->m_logistCost = i->m_cargonDistance * 5 * i->m_cargoNum * 1000 * 10/10000000;
        else
            i->m_logistCost = i->m_cargonDistance * 10* i->m_cargoNum * 1000 * 10/10000000;
//        cout<<i->m_cargonDistance<<"     "<<i->m_cargoNum<<endl;
        outfile<<i->m_label<<","<<i->m_logistCost<< ",";
        //输出路径
        for(auto s: i->m_cargonRoute)
            outfile<<s<<",";
        outfile<<endl;
    }

}
int main()
{
    vector<CNode> nodeVec;                  //存放节点集合
    vector<CLink> linkVec;                  //存放链接集合
    InputData(nodeVec, linkVec);
    cout<< "节点数量为： "<< nodeVec.size() << "   链接数量为： " << linkVec.size() << endl;


    ChooseCargonRoute(nodeVec, linkVec);
//    PrintPath(nodeVec);
    deque<size_t> greedyResult = Greedy(nodeVec,linkVec);

    ofstream outfile("排序.csv");
    outfile<<"城市1"<<","<<"城市2"<<endl;
    for(auto i: greedyResult)
    {
        outfile<< linkVec[i].m_node1 << "," << linkVec[i].m_node2 << endl;
    }

    double cost = GetCost(nodeVec, linkVec, greedyResult);
    outfile<<"\n\n\n";
    outfile<<"城市编号"<<","<<"金额（千万）"<<endl;
    for(auto i: nodeVec)
        outfile<<i.m_label<<","<<i.m_logistCost/10000000<<endl;
    cout<< cost/10000000 << endl;




    system("pause");
    return 0;
}
