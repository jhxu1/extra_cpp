#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <algorithm>

#include "Node.h"
#include "tool.h"
#include "ECgame.h"
#include "main.h"
#include "GA.h"

using namespace std;

const string node_file = "sc-free.txt";
const float final_point = 0.41;
const float first_point = 0.4;

float frac = first_point;
const int subsidyNum = 5;
float taxW =3;

const int MAXTIME = 100;

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

vector<int> load_min_degree_sort(NE &ne)
{
    vector<int> degree_sort;
    vector<Node*> temp = ne.nodes;
    sort(temp.begin(), temp.end(), [](Node* &node1, Node* &node2){return node1->degree()<node2->degree();});
    for(auto i:temp)
        degree_sort.push_back(i->getFlag());
    return degree_sort;
}

vector<int> degree2(NE &ne)
{
    vector<Node*> candi;
    vector<Node*> temp = ne.nodes;
    sort(temp.begin(), temp.end(), [](Node* &node1, Node* &node2){return node1->degree()>node2->degree();});
    int times = 0;
    for(auto i:temp)
    {
        bool flag = true;
        for(auto j:candi)
        {
            if(i->ifNei(j))
            {
                flag = false;
                break;
            }
        }
        if(flag == true)
        {
            candi.push_back(i);
            if(times == subsidyNum)
                break;
        }
    }
    vector<int> result;
    for(auto i:candi)
        result.push_back(i->getFlag());
    return result;
}

vector<int> load_sort(NE &ne, const string fileName)
{
    vector<int> result;
    ifstream infile(fileName);
    int a;
    while(infile >> a)
        result.push_back(a);
    return result;
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
    for(auto i:minNE)
        cout<<i<<"\t"<<ne.nodes[i]->degree()<<endl;
    //sort(minNE.begin(),minNE.end(),[&ne](int n1, int n2){return ne.nodes[n1]->degree() < ne.nodes[n2]->degree();});
    //洗扑克
    for(int i=0;i<minNE.size();i++)
    {
        uniform_int_distribution<int> dis(i, minNE.size()-1);
        swap(minNE[i], minNE[dis(engine)]);
    }

    return minNE;
}

//补助最优纳什中接种的，最差纳什中不接种的，选择部分根据度剩余的随机
vector<int> subsidy_method3(NE &ne, double T)
{
    ofstream outfile("subsidy_method3_info.txt",ios::app);
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
    //outfile<<"T:\t"<<T<<"size: \t"<<minNE.size()<<endl;
//    for(auto i:minNE)
//        outfile<<i<<"\t"<<ne.nodes[i]->degree()<<endl;
    sort(minNE.begin(),minNE.end(),[&ne](int n1, int n2){return ne.nodes[n1]->degree() > ne.nodes[n2]->degree();});
    //int k = ceil(subsidyNum*1.0/2);
    int k = 0;
    //洗扑克

    int Allresult = 0;
    for(int t=0;t<30;t++)
    {
        for(int i=k;i<minNE.size();i++)
        {
            uniform_int_distribution<int> dis(i, minNE.size()-1);
            swap(minNE[i], minNE[dis(engine)]);
        }
        outfile<<"---------------"<<endl;
        for(int i=0;i<subsidyNum;i++)
            outfile<<minNE[i]<<"\t"<<ne.nodes[minNE[i]]->degree()<<endl;
        ne.init();
        subsidyProcess(ne, minNE);
        int result = maxNE(ne,T);
        outfile<<t<<"\t"<<result<<endl;
        Allresult += result;
    }
    Allresult /= 30;
    outfile<<"------总结果-------"<<endl;
    outfile<<T<<"\t"<<Allresult<<endl;
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
	ofstream outfile("degree.csv");
	Tool::outdegree(outfile, ne.nodes);
	outfile.close();

	ofstream outfilemin("minNE.txt");
	ofstream outfilemax("maxNE.txt");
	while(frac<final_point)
    {
        int result = 1000000;
        double t = ne.getLambda1()*frac;
        cout<<"****threshold: "<<t<<"  ****"<<endl;
        vector<int> subsidy_index;
        GA(ne,t);
//        for(int time = 0;time<MAXTIME;time++)
//        {
//            //vector<int> subsidy_index = load_degree_sort(ne);
//            //subsidy_index = load_min_degree_sort(ne);
//            //vector<int> subsidy_index = load_sort(ne, "kshell_sort.txt");
//            //vector<int> subsidy_index = subsidy_method2(ne,t);
//            //subsidy_index = degree2(ne);
//            subsidy_index = random_subsidy(ne);
//            //      ne.init();
//            //      subsidyProcess(ne, subsidy_index);
//            //      outfilemin<<t<<"\t"<<ne.HDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
//            ne.init();
//            subsidyProcess(ne, subsidy_index);
//            int num = maxNE(ne,t);
//            if(result > num)
//                result = num;
//            cout<<"Current time:\t"<<time<<"\tCurrent threshold T:\t"<<t<<"\tThe Max NE Cost:\t"<<num<<endl;
//        }
//        outfilemax<<t<<"\t"<<result+min(int(subsidy_index.size()),subsidyNum)<<endl;
//        //outfilemax<<t<<"\t"<<ne.LDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
//        //get_degree_info(ne);
        frac+=0.05;
    }



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
