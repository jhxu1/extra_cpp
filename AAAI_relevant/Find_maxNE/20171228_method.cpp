#include "Node.h"
#include "ECgame.h"
#include "tool.h"
#include "20171228_method.h"
#include <map>

using namespace std;

//找最大度
Node* max_degree(vector<Node*> nodes)
{
    Node* maxDegree_Node = nullptr;
    int maxD = 0;
    for(auto i:nodes)
    {
        if(i->degree() > maxD)
        {
            maxDegree_Node = i;
            maxD = maxDegree_Node->degree();
        }

    }
    return maxDegree_Node;
}

////找连接数最少的节点集合（度大于2）
//vector<Node*> get_min_link_node(vector<Node*> nodes,  map<Node*, int> link_num,  map<Node*, bool> ifchoosed)
//{
//    vector<Node*> min_link_node;
//    int min_link = ECgame::size;
//    vector<Node*> candidate;
//    for(auto i:nodes)
//    {
//        if(i->degree() > 1 && ifchoosed[i] == false)
//            candidate.push_back(i);
//    }
//    for(auto i:candidate)
//    {
//        if(link_num[i] !=0 && link_num[i] < min_link)
//            min_link = link_num[i];
//    }
//    if(min_link == ECgame::size)
//        min_link = 0;
//    for(auto i:candidate)
//        if(link_num[i] == min_link)
//            min_link_node.push_back(i);
//    return min_link_node;
//}

//找连接数最duoduodduodudodudoudo的节点集合（度大于2）
vector<Node*> get_min_link_node(vector<Node*> nodes,  map<Node*, int> link_num,  map<Node*, bool> ifchoosed)
{
    vector<Node*> min_link_node;
    int min_link = 0;
    vector<Node*> candidate;
    for(auto i:nodes)
    {
        if(i->degree() > 1 && ifchoosed[i] == false)
            candidate.push_back(i);
    }
    for(auto i:candidate)
    {
        if( link_num[i] > min_link)
            min_link = link_num[i];
    }
    if(min_link == ECgame::size)
        min_link = 0;
    for(auto i:candidate)
        if(link_num[i] == min_link)
            min_link_node.push_back(i);
    return min_link_node;
}

//找接种节点
vector<Node*> get_vacc_nodes(vector<Node*> nodes,  map<Node*, int> link_num,  map<Node*, bool> ifchoosed)
{
    vector<Node*> min_link_node;
    int min_link = 0;
    vector<Node*> candidate;
    for(auto i:nodes)
    {
        if( ifchoosed[i] == false)
            candidate.push_back(i);
    }
    return candidate;
}


//判断节点是否为已影响节点
bool ifInfluenced(Node* n)
{
    if(n->strategies == 1)
        return true;
    for(auto i:n->nei())
    {
        if(i->strategies == 1)
            return true;
    }
    return false;
}

//寻找节点n的未影响邻居数
int UnInfluNei(Node* n)
{
    int num = 0;
    for(auto i:n->nei())
    {
        if(ifInfluenced(i) == false)
            num++;
    }
    return num;
}

//获取最大的未影响邻居数节点
vector<Node*> get_max_UnInfluNei_nodes( vector<Node*> min_link_node)
{
    map<Node*, int> UninfluNei_num;
    vector<Node*> max_UnInfluNei_nodes;
    for(auto i:min_link_node)
        UninfluNei_num[i] = UnInfluNei(i);
    int maxNum = -100;
    for(auto i:min_link_node)
    {
        if(UninfluNei_num[i] > maxNum)
            maxNum = UninfluNei_num[i];
    }
    for(auto i:min_link_node)
    {
        if(UninfluNei_num[i] == maxNum)
            max_UnInfluNei_nodes.push_back(i);
    }
    return max_UnInfluNei_nodes;
}

//得到节点n的分数
int score(Node* n)
{
    int s = 0;
    for(auto i:n->nei())
    {
        if(ifInfluenced(i) == false)
            s += UnInfluNei(i);
    }
    return s;
}

Node* get_max_score(vector<Node*> max_UnInfluNei_nodes)
{
    int max_score = -100;
    Node* max_score_node = nullptr;
    for(auto i:max_UnInfluNei_nodes)
    {
        int s = score(i);
        if(s > max_score)
        {
            max_score_node = i;
            max_score = s;
        }
    }
    return max_score_node;
}
/*
方法：
1.	所有个体处于接种状态。
2.	首先判断度最大的节点是否可以不接种（因为第一个点，肯定可以），形成初始簇C。
3.	判断网络中的节点与簇C的连接数，选择最小连接数的节点（度需要大于等2）。
    3.1	若最小连接数相同：则选取周围未被影响到的接种节点数量多的。
    3.2	若未被影响到的接种节点也相同：则计算两个节点的分数大小。
*/
int cluster_improved(vector<Node*> nodes, double T)
{
    int size = ECgame::size;
    map<Node*, bool> ifchoosed;
    map<Node*, int> link_num;
    //全部个体接种
    for(auto i:nodes)
    {
        i->strategies = 0;
        link_num[i] = 0;
        ifchoosed[i] = false;
    }
    //寻找度最大的点
    Node* first_node = max_degree(nodes);
    first_node->strategies = 1;
    for(auto i:first_node->nei())
        link_num[i] ++;
    ifchoosed[first_node] = true;
    cout<<"first:   "<<first_node->getFlag()<<endl;
    while(true)
    {
        Node* target_node = nullptr;
        //找连接数最少的节点
        vector<Node*> min_link_node = get_vacc_nodes(nodes, link_num, ifchoosed);
        cout<<"size of min_link_node:      "<<min_link_node.size()<<endl;
//        //-----
//        for(auto i:min_link_node)
//            cout<<i->getFlag()<<"  ";
//        cout<<endl;
//        //----
        if(min_link_node.size() == 0)                                       //只剩下叶子节点了
            break;
        else if(min_link_node.size() == 1)
            target_node = min_link_node[0];
        else
        {
            vector<Node*> max_UnInfluNei_nodes = get_max_UnInfluNei_nodes(min_link_node);
            cout<<"size of max_UnInfluNei_nodes:      "<<max_UnInfluNei_nodes.size()<<endl;
//            //-----
//            for(auto i:max_UnInfluNei_nodes)
//                cout<<i->getFlag()<<"  ";
//            cout<<endl;
//            //----
            if(max_UnInfluNei_nodes.size() == 1)
                target_node = max_UnInfluNei_nodes[0];
            else
                target_node = get_max_score(max_UnInfluNei_nodes);
        }
        //对目标节点进行处理
        cout<<"target_Node:   "<<target_node->getFlag();
        ifchoosed[target_node] = true;
        target_node->strategies = 1;
        double l = Tool::getMaxEigen(nodes);
        if(l > T)
        {
            target_node->strategies = 0;
            cout<<"  vacc  ";
        }
        else
        {
            cout<<"  unvacc  ";
            for(auto i:target_node->nei())
                link_num[i] ++;
        }
        cout<<endl;
        cout<<"lambda:   "<<l<<endl;
        cout<<"----------------------------------------------------"<<endl;
    }
    //对叶子节点进行处理
    for(auto i:nodes)
    {
        if(ifchoosed[i] == false)
        {
            if(i->degree() != 1)
                cout<<"error!!!!!!!!"<<endl;
            cout<<"leaf node:  "<<i->getFlag();
            i->strategies = 1;
            double l = Tool::getMaxEigen(nodes);
            if(l > T)
            {
                i->strategies = 0;
                cout<<"  vacc  ";
            }
            else
                cout<<"  unvacc  ";
            cout<<endl;

        }
    }
    int num = count_if(nodes.begin(),nodes.end(),[](Node* n){return n->strategies == 0;});
    return num;
}
