#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include "Node.h"
#include "tool.h"
#include "ECgame.h"
#include "main.h"

using namespace std;

const string node_file = "1000sc-free.txt";
const float final_point = 1;
const float first_point = 0.2;

float frac = first_point;
const int subsidyNum = 5;
float taxW =3;

default_random_engine engine(time(nullptr));

void subsidyProcess(NE &ne, vector<int> &subsidy_index);
vector<int> load_EI_sort()
{
    ifstream infile("EI_sort.txt");
    vector<int> EI_sort;
    int temp;
    while(infile>>temp)
        EI_sort.push_back(temp);
    return EI_sort;
}

vector<int> load_degree_sort(NE &ne)
{
    vector<int> degree_sort;
    vector<Node*> temp = ne.nodes;
    sort(temp.begin(), temp.end(), [](Node* &node1, Node* &node2){return node1->degree()>node2->degree();});
    for(auto i:temp)
        degree_sort.push_back(i->getFlag());
    return degree_sort;
}

vector<int> random_subsidy(NE &ne)
{
    vector<int> result;
    for(int i=0;i<ECgame::size;i++)
        result.push_back(i);
    //洗扑克
    for(int i=0;i<ECgame::size;i++)
    {
        uniform_int_distribution<int> dis(i, ECgame::size-1);
        swap(result[i], result[dis(engine)]);
    }
    return result;
}

//补助最优纳什中接种的，最差纳什中不接种的
vector<int> subsidy_method2(NE &ne, double T)
{
    //选择最优纳什接种最差纳什不接种个体
    vector<int> minNE;
    ne.init();
    ne.HDG(T);
    for(auto i:ne.nodes)
    {
        if(i->strategies == 0)
            minNE.push_back(i->getFlag());
    }
    ne.init();
    maxNE(ne,T);
    for(auto it=minNE.begin();it!=minNE.end();)
    {
        if(ne.nodes[*it]->strategies == 0 && minNE.size() > subsidyNum)
            it = minNE.erase(it);
        else
            it++;
    }
    //sort(minNE.begin(),minNE.end(),[&ne](int n1, int n2){return ne.nodes[n1]->degree() > ne.nodes[n2]->degree();});
    //洗扑克
    for(int i=0;i<minNE.size();i++)
    {
        uniform_int_distribution<int> dis(i, minNE.size()-1);
        swap(minNE[i], minNE[dis(engine)]);
    }

    return minNE;
}

//赌轮法，给定一个待选集合和概率
int bet_wheel(vector<double> &score)
{
    double sumScore = accumulate(score.begin() , score.end() , 0.0);
    int cand_size = score.size();
    vector<float> pro(cand_size, 0);
    pro[0] = score[0]/sumScore;
    for(int i=1;i<cand_size;i++)
    {
        pro[i] = pro[i-1] + score[i]/sumScore;
    }
    //生成随机数
    uniform_real_distribution<float> dis(0, 1);
    float ran = dis(engine);
    for(int i=0;i<cand_size;i++)
    {
        if(ran<pro[i])
        {
            return i;
        }
    }
}
void iter_subsidy(NE &ne, double T)
{
    ofstream outfile("iter_info.txt");
    int NosubsidyCost = 0;
    double stepLength = 0.5;
    int MAXTIME = 50;
    //选择最优纳什接种最差纳什不接种个体
    vector<int> minNE;
    ne.init();
    ne.HDG(T);
    for(auto i:ne.nodes)
    {
        if(i->strategies == 0)
            minNE.push_back(i->getFlag());
    }
    ne.init();
    NosubsidyCost = maxNE(ne,T);
    outfile<<"No subsidy max cost:   "<<NosubsidyCost<<endl;
    for(auto it=minNE.begin();it!=minNE.end();)
    {
        if(ne.nodes[*it]->strategies == 0 && minNE.size() > subsidyNum)
            it = minNE.erase(it);
        else
            it++;
    }
    //计算分数
    int cand_size = minNE.size();
    outfile<<"cand_size:    "<<cand_size<<endl;
    map<int, int> dict;
    for(int i=0;i<cand_size;i++)
    {
        dict[minNE[i]] = i;
    }
    //如果集合中个数小于补助人数
    if(cand_size <= subsidyNum)
    {
        outfile<<"补助人数小于待选节点个数";
        outfile<<subsidyNum<<endl;
        return;

    }
    //计算度补助
    ne.init();
    vector<int> temp = load_degree_sort(ne);
    subsidyProcess(ne, temp);
    int DegCost = maxNE(ne, T);
    outfile<<"DegCost:\t"<<DegCost<<endl;
    for(auto i:temp)
        outfile<<i<<"\t";
    outfile<<endl;

    double sumScore = cand_size;
    vector<double> score(cand_size,1);
    for(int t=0;t<MAXTIME;t++)
    {
        vector<int> subVec(subsidyNum, -1);
        //赌轮法
        vector<double> temp_score = score;
        for(int s=0;s<subsidyNum;s++)
        {
            int index = bet_wheel(temp_score);
            temp_score.erase(temp_score.begin()+index);
            int num = count_if(subVec.begin(),subVec.end(),[&dict,index](int n1){return n1 != -1 && dict[n1]<=index;});
            index += num;
            index = minNE[index];
            subVec[s] = index;
        }
        //计算最差纳什花费
        ne.init();
        subsidyProcess(ne, subVec);
        int maxNeCost = maxNE(ne, T);
        outfile<<t<<"\t";
//        for(auto i:subVec)
//            outfile<<i<<"\t";
        outfile<<maxNeCost<<endl;
        //更新score
        sumScore = 0;
        for(int i=0;i<subsidyNum;i++)
        {
            int index = subVec[i];
            int id = dict[index];
            sumScore -= score[id];
            cout<<index<<"-----"<<id<<"------"<<score[id]<<"\t\t";
            score[id] += stepLength * (-maxNeCost + NosubsidyCost)/NosubsidyCost;
            if(score[id]<0)
                score[id] = 0;
            sumScore += score[id];
            cout<<index<<"-----"<<id<<"------"<<score[id]<<endl;
        }
    }
}

void iter_subsidy2(NE &ne, double T)
{
    ofstream outfile("iter_info.txt");
    int NosubsidyCost = 0;
    double tao = 0.5;
    int MAXTIME = 3000;
    ne.init();
    NosubsidyCost = maxNE(ne,T);
    outfile<<"无补助花费： "<<NosubsidyCost<<endl;
    //选择最优纳什接种最差纳什不接种个体
    vector<int> minNE;
//    ne.init();
//    ne.HDG(T);
//    for(auto i:ne.nodes)
//    {
//        if(i->strategies == 0)
//            minNE.push_back(i->getFlag());
//    }
//    ne.init();
//    NosubsidyCost = maxNE(ne,T);
//    outfile<<"No subsidy max cost:   "<<NosubsidyCost<<endl;
//    for(auto it=minNE.begin();it!=minNE.end();)
//    {
//        if(ne.nodes[*it]->strategies == 0 && minNE.size() > subsidyNum)
//            it = minNE.erase(it);
//        else
//            it++;
//    }
//
//    sort(minNE.begin(),minNE.end(),[&ne](int n1, int n2){return ne.nodes[n1]->degree() > ne.nodes[n2]->degree();});
//    ne.init();
//    subsidyProcess(ne, minNE);
//    int DegCost2 = maxNE(ne, T);
//    outfile<<"DegCost2:\t"<<DegCost2<<endl;
//    for(auto i:minNE)
//        outfile<<i<<"\t";
//    outfile<<endl;

    //尝试从所有数据中获取
    minNE.resize(ECgame::size);
    for(int i=0;i<minNE.size();i++)
        minNE[i] = i;
    //获取字典
    int cand_size = minNE.size();
    outfile<<"cand_size:    "<<cand_size<<endl;
    map<int, int> dict;
    for(int i=0;i<cand_size;i++)
    {
        dict[minNE[i]] = i;
    }
    //如果集合中个数小于补助人数
    if(cand_size <= subsidyNum)
    {
        outfile<<"补助人数小于待选节点个数";
        outfile<<subsidyNum<<endl;
        return;

    }
    //计算度补助
    ne.init();
    vector<int> temp = load_degree_sort(ne);
    subsidyProcess(ne, temp);
    int DegCost = maxNE(ne, T);
    outfile<<"DegCost:\t"<<DegCost<<endl;
    for(auto i:temp)
        outfile<<i<<"\t";
    outfile<<endl;

    vector<double> Q(cand_size,0);
    int maxNECost, lastMaxNECost;
    lastMaxNECost = NosubsidyCost;
    for(int t=0;t<MAXTIME;t++)
    {
        vector<int> subVec(subsidyNum, -1);
        //计算sumQ
        vector<double> element(cand_size, 0);
        double sumQ = 0;
        for(int i=0;i<cand_size;i++)
        {
            element[i] = exp(Q[i]/tao);
            sumQ += element[i];
        }
        //计算每个个体的概率
        vector<double> pro(cand_size, 0);
        for(int i=0;i<cand_size;i++)
            pro[i] = element[i] / sumQ;
        for(int s=0;s<subsidyNum;s++)
        {
            int index = bet_wheel(pro);
            pro.erase(pro.begin()+index);
            vector<int> temp_subVec = subVec;
            sort(temp_subVec.begin(),temp_subVec.end(),[&dict](int n1,int n2){return dict[n1]<dict[n2];});
            for(int s1 =0;s1<s;s1++)
            {
                int in = temp_subVec[s1];
                if(in!=-1 && dict[in] <= index)
                    index++;
            }
            cout<<"subsidy num: "<<s<<"  index: "<<index<<endl;
            index = minNE[index];
            subVec[s] = index;
        }
        //计算最差纳什花费
        ne.init();
        subsidyProcess(ne, subVec);
        maxNECost = maxNE(ne, T);
        outfile<<t<<"\t";
        for(auto i:subVec)
            outfile<<i<<"\t";
        outfile<<maxNECost<<endl;
        //更新花费
        for(int i=0;i<subsidyNum;i++)
        {
            int index = subVec[i];
            int id = dict[index];
            cout<<index<<"-----"<<id<<"------"<<Q[id]<<"\t\t";
            Q[id] = (1.0/(t+2))*((t+1)*Q[id]+(lastMaxNECost - maxNECost));
            cout<<index<<"-----"<<id<<"------"<<Q[id]<<endl;
        }
    }
}

void subsidyProcess(NE &ne, vector<int> &subsidy_index)
{
    vector<Node*> subsidyNode;
    for(int i=0;i<min(subsidyNum,int(subsidy_index.size()));i++)
    {
        int index = subsidy_index[i];
        cout<<index<<endl;
        subsidyNode.push_back(ne.nodes[index]);
    }
    ne.subsidy(subsidyNode);
}

vector<int> taxSubsidy(NE &ne,double T)
{
    vector<int> subsidy_index;
    int size = ne.nodes.size();
    //找最大度
    int maxD = 0;
    for(auto i:ne.nodes)
    {
       if(i->degree() > maxD)
            maxD = i->degree();
    }
    //计算tax
    for(auto i:ne.nodes)
    {
        double tax = i->degree()/maxD * ECgame::Le * (1-T/ne.getLambda1()) * taxW;
        if(tax > ECgame::C)
        {
            subsidy_index.push_back(i->getFlag());
        }
    }
    return subsidy_index;
}

vector<int> DegSubsidy2(NE &ne, int K, double T)
{
    //选择最优纳什接种最差纳什不接种个体
    vector<int> minNE;
    ne.init();
    ne.HDG(T);
    for(auto i:ne.nodes)
    {
        if(i->strategies == 0)
            minNE.push_back(i->getFlag());
    }
    ne.init();
    maxNE(ne,T);
    for(auto it=minNE.begin();it!=minNE.end();)
    {
        if(ne.nodes[*it]->strategies == 0 && minNE.size() > subsidyNum)
            it = minNE.erase(it);
        else
            it++;
    }
    if(minNE.size()<subsidyNum)
        return minNE;
    map<int, int> dict;
    vector<int> k_eff(minNE.size(),0);
    for(int i=0;i<minNE.size();i++)
    {
        int target = minNE[i];
        dict[target] = i;
        k_eff[i] = ne.nodes[minNE[i]]->degree();
        for(auto j:ne.nodes[minNE[i]]->nei())
        {
            if(j->degree() > K)
                k_eff[i] --;
        }
    }
    sort(minNE.begin(),minNE.end(),[&dict, &k_eff](int n1,int n2){return k_eff[dict[n1]] > k_eff[dict[n2]];});
    return minNE;
}


//---------查找接种节点的度
void get_degree_info(NE &ne)
{
    ofstream outfile("degree_info.csv");
    vector<Node*> vec= ne.nodes;
    sort(vec.begin(),vec.end(),[](Node* n1, Node* n2){return n1->degree()>n2->degree();});
    for(auto i:vec)
        outfile<<i->getFlag()<<","<<i->degree()<<","<<i->strategies<<endl;
    outfile.close();
}
int main()
{
	//used to calculate time
	clock_t start, finish;
	double totaltime;
	start = clock();

	//ґ¦АнЅЪµг
	NE ne(node_file);
//	ofstream outfile("degree.csv");
//	Tool::outdegree(outfile, ne.nodes);
//	outfile.close();

	ofstream outfilemin("minNE.txt");
	ofstream outfilemax("maxNE.txt");
	while(frac<final_point)
    {
        double t = ne.getLambda1()*frac;
        cout<<"****threshold: "<<t<<"  ****"<<endl;
        //vector<int> subsidy_index = load_degree_sort(ne);
        vector<int> subsidy_index = DegSubsidy2(ne, 6, t);
        //vector<int> subsidy_index = subsidy_method2(ne,t);
        //vector<int> subsidy_index = random_subsidy(ne);
        //vector<int> subsidy_index = taxSubsidy(ne, t);
//        ne.init();
//        subsidyProcess(ne, subsidy_index);
//        outfilemin<<t<<"\t"<<ne.HDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        ne.init();
        subsidyProcess(ne, subsidy_index);
        outfilemax<<t<<"\t"<<maxNE(ne,t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        //outfilemax<<t<<"\t"<<ne.LDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        get_degree_info(ne);
        frac+=0.1;
    }



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
