#include "ECgame.h"


using namespace std;

class Node;

const double ECgame::C = 0.5;             //接种花费
const double ECgame::L = 0;             //未接种未染病花费
const double ECgame::Le = 1;            //染病花费
const int ECgame::size = 100;          //节点个数
const float ECgame::learning_rate = 0.2;	//RLA算法学习率
double ECgame::T = 0;


void ECgame::readData(const std::string &filename)
{
	nodes = inputData(filename);
	lambda1 = Tool::getMaxEigen(nodes);
	//cout << "lambda1:  " << lambda1 << endl;
}

//void NE::remove_flag(int index, vector<Node*> &pai)
//{
//    for (int j = 0; j < pai.size(); j++)
//    {
//        if (pai[j]->flag > pai[index]->flag)
//        {
//            pai[j]->flag--;
//        }
//    }
//    pai[index]->flag = -1;
//}

//void NE::add_flag(int index, int flag, vector<Node*> &pai)
//{
//    int f = flag; int count = 0;
//    for (int j = 0; j < pai.size(); j++)
//    {
//        if (pai[j]->fixflag < f && pai[j]->strategies == 0)
//            count++;
//    }
//    pai[index]->flag = f-count;
//    for (int j = 0; j < pai.size(); j++)
//    {
//        if (pai[j]->flag >= pai[index]->flag && j!=index)
//        {
//            pai[j]->flag++;
//        }
//    }
//}

double NE::iterative_secure(double t,vector<Node*> &pai)
{
	T = t; size_t size = nodes.size();


	double l = Tool::getMaxEigen(pai);
	cout<<"After subsidy MaxEigen: "<<l<<endl;
	size_t num = 0;
	double lastl = l;
	int index = pai.size()-1;
	cout<<"---------stage 1-----------"<<endl;
	while (l > T || l==-1)
	{
	    if(pai[index]->strategies == 1)
            pai[index]->strategies = 0;
        else
        {
            {
                index--;
                continue;
            }
        }


		l = Tool::getMaxEigen(pai);
		if(l==-1)
            l = lastl;
        lastl = l;
		num++;
		cout<<"<<stage 1>>   num:  "<<num<<"\tmaxeigenV:  "<<l<<endl;
		index --;
	}
	cout<<"---------stage 2-----------"<<endl;
	index += 2;
    if(index >= pai.size())
    {
        int vacc_num = count_if(pai.begin(), pai.end(), [](Node* n){return n->strategies == 0;});
//		restrain_flag();                //重置编号
		for (auto i : pai)				//重置策略
			i->strategies = 1;
        return (vacc_num)*1.0;
    }

    while(index < pai.size())
    {
        num--;
        pai[index]->strategies = 1;
        //恢复节点编号
//        add_flag(index,pai[index]->fixflag,pai);

        l = Tool::getMaxEigen(pai);
        if(l==-1)
            l = lastl;
        lastl = l;
        //若去接种节点之后，特征值大于T，则重新接种该节点
        if(l > T)
        {
            num++;
            pai[index]->strategies = 0;
            //移除节点编号
//            remove_flag(index,pai);
        }
        cout<<"<<stage 2>>   num:  "<<num<<"\tmaxeigenV:  "<<l<<endl;
        index++;
    }
	int vacc_num = count_if(pai.begin(), pai.end(), [](Node* n){return n->strategies == 0;});
//    restrain_flag();                //重置编号
	for (auto i : pai)
	{
	    if(i->strategies ==0)
            vacc_info.push_back(i->getFlag());
	}

	return (vacc_num)*1.0;
}

double NE::HDG(double t)
{
    cout<<"--------------------------HDG------------------------"<<endl;
    vector<Node*> pai = nodes;
    for(auto it=pai.begin();it!=pai.end();)
    {
        if((*it)->ifGetSubsidy == true)
            it = pai.erase(it);
        else
            it++;
    }
	sort(pai.begin(), pai.end(), [](Node *n1, Node *n2) {return n1->degree() < n2->degree(); });//from min to max
    return iterative_secure(t, pai);
}

double NE::LDG(double t)
{
    cout<<"--------------------------LDG------------------------"<<endl;
    vector<Node*> pai = nodes;
    for(auto it=pai.begin();it!=pai.end();)
    {
        if((*it)->ifGetSubsidy == true)
            it = pai.erase(it);
        else
            it++;
    }
	sort(pai.begin(), pai.end(), [](Node *n1, Node *n2) {return n1->degree() > n2->degree(); });//from min to max
    return iterative_secure(t, pai);
}

void NE::RLA_algorithm(double T)
{
	//1) Set an initial probability pi(0), for each user i =1,...,N.
	size_t size = nodes.size();
	vector<float> pi(size, 0.5);
	vector<double> payoff(size);
	int t = 0;
	default_random_engine engine(time(nullptr));		//随机数种子
	uniform_real_distribution<double> u(0, 1);
	while (true)
	{
		for (int i = 0; i < size; i++)
		{
			// 2) At every time step t, each node i invests with probability
			// pi(t).This determines his pure action σi(t).
			if (u(engine) > pi[i])
				nodes[i]->strategies = 0;
			else
				nodes[i]->strategies = 1;
		}
		// 3) Each player i obtains his payoff Si(σi(t); σ ¯(t))
		double l = Tool::getMaxEigen(nodes);
		for (int i = 0; i < size; i++)
		{
			if (nodes[i]->strategies == 0)
				payoff[i] = L;
			else
				payoff[i] = l < T ? 0 : Le;
			payoff[i] /= L + Le;
		}
		// 4) Each node i updates its probability according to the following rule :
		for (int i = 0; i < size; i++)
		{
			pi[i] = pi[i] + learning_rate * (1-payoff[i]) * (1 - nodes[i]->strategies - pi[i]);
		}
	}
}



//重置nodes的编号
//void NE::restrain_flag()
//{
//    for(int i=0;i<nodes.size();i++)
//        nodes[i]->flag = nodes[i]->fixflag;
//}

//补助
void NE::subsidy(vector<Node*> pai)
{
    for(auto i:pai)
    {
        i->ifGetSubsidy = true;
        i->strategies = 0;
    }
}

void NE::init()
{
    vacc_info.clear();
	for (auto i : nodes)
	{
		i->flag = i->fixflag;
		i->strategies = 1;
		i->ifGetSubsidy = false;
	}
}

//最终确定方案----------------------------------------
//判断节点是否为已影响节点
bool ifInfluenced2(Node* n, map<Node*, bool> &ifchoosed)
{
    if(n->ifGetSubsidy==true || n->strategies == 1 || ifchoosed[n] == true)
        return true;
    for(auto i:n->nei())
    {
        if(i->ifGetSubsidy == false && i->strategies == 1)
            return true;
    }
    return false;
}

//寻找节点n的未影响邻居数
int UnInfluNei2(Node* n, map<Node*, bool> &ifchoosed)
{
    int num = 0;
    for(auto i:n->nei())
    {
        if(ifInfluenced2(i, ifchoosed) == false)
            num++;
    }
    return num;
}

//邻居中未接种个数
int clusterNei(Node *n)
{
    int num = 0;
    for(auto i:n->nei())
    {
        if(i->strategies == 1)
            num++;
    }
    return num;
}
//得到节点n的分数
int Soc(Node* n, map<Node*, bool> &ifchoosed)
{
    int s = 0;
    for(auto i:n->nei())
    {
        if(ifInfluenced2(i, ifchoosed) == false)
            s += UnInfluNei2(i, ifchoosed);
    }
    return s;
}


bool ifchange_flag(vector<Node*> nodes, map<Node*, bool> ifchoosed)
{
    vector<Node*> unvacc;
    for(auto i:nodes)
    {
        if(i->strategies == 1)
        {
            for(auto j:i->nei())
            {
                if(ifchoosed[j] == false)
                    return false;
            }
        }
    }
    return true;
}

int maxNE(NE &ne, double T)
{
    float frac = T/ne.lambda1;
    map<Node*, bool> ifchoosed;
    map<Node*, double> score;
    bool flag = false;              //用来标记是否有可选节点。如果没有选择影响节点最大的点
    vector<Node*> nodes;
    for(auto it:ne.nodes)
    {
        if(it->ifGetSubsidy == false)
            nodes.push_back(it);
        else
            ifchoosed[it] = true;
    }
    double l = Tool::getMaxEigen(nodes);
	//cout<<"After subsidy MaxEigen: "<<l<<endl;
    for(auto i:nodes)
    {
        i->strategies = 0;
        ifchoosed[i] = false;
        score[i] = -1;
    }
    for(int t = 0;t < nodes.size() ;t++)
    {
        Node* target_node = nullptr;
        vector<Node*> candi;
        if(flag == false)
        {
            for(auto i:nodes)
            {
                if(ifchoosed[i] == false)
                {
                    score[i] = UnInfluNei2(i, ifchoosed);
                    candi.push_back(i);
                }
            }
            flag = true;
        }
        else
        {
            //计算与簇相连的每个个体的分数
            for(auto i:nodes)
            {
                if(ifchoosed[i] == false)
                {
                    int n2 = clusterNei(i), n1 = UnInfluNei2(i, ifchoosed);
                    if(n2 != 0)                     //n1 = 0 的时候没处理
                    {
                        double f = frac;
                        score[i] = f * n2 + (1-f) * n1 + 0.001*Soc(i, ifchoosed);
                        candi.push_back(i);
                    }
                }
            }
        }
        double max_score = -1;
        for(auto i:candi)
        {
            if(ifchoosed[i] == false && score[i] > max_score)
            {
                max_score = score[i];
                target_node = i;
            }
        }
        //cout<<"target_Node:   "<<target_node->getFlag()<<"  score:  "<<score[target_node]<<"\t";
        ifchoosed[target_node] = true;
        target_node->strategies = 1;
        double l = Tool::getMaxEigen(nodes);
        if(l > T)
        {
            target_node->strategies = 0;
            //cout<<"  vacc  ";
        }
        else
        {
            //cout<<"  unvacc  ";
        }
        //cout<<endl;
        //cout<<"lambda:   "<<l<<endl;
        //cout<<"----------------------------------------------------"<<endl;
        if(ifchange_flag(nodes, ifchoosed))
            flag = false;               //从true变为false
    }
    int num = count_if(nodes.begin(),nodes.end(),[](Node* n){return n->strategies == 0;});
        return num;
}
