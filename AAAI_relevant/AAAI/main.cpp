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

default_random_engine engine(time(nullptr));

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

//补助最优纳什中接种的，最差纳什中不接种的
vector<int> subsidy_method2(NE &ne, double T)
{
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
    //洗扑克
    for(int i=0;i<minNE.size();i++)
    {
        uniform_int_distribution<int> dis(i, minNE.size()-1);
        swap(minNE[i], minNE[dis(engine)]);
    }

    return minNE;
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
        vector<int> subsidy_index = subsidy_method2(ne,t);
        ne.init();
        subsidyProcess(ne, subsidy_index);
        outfilemin<<t<<"\t"<<ne.HDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        ne.init();
        subsidyProcess(ne, subsidy_index);
        //outfilemax<<t<<"\t"<<maxNE(ne,t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        outfilemax<<t<<"\t"<<ne.LDG(t)+min(int(subsidy_index.size()),subsidyNum)<<endl;
        frac+=0.1;
    }



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
