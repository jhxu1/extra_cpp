#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include "Node.h"
#include "tool.h"
#include "ECgame.h"
#include "main.h"

using namespace std;

const string node_file = "123.txt";
const float final_point = 1;
const float first_point = 0.2;

float frac = 0.2;
const int subsidyNum = 2;

/*
功能：枚举num个人接种的情况
*/
void enumerate(ostream &outfile, NE &ne, int num, vector<int> index, vector<int> vacc_index)
{
    if(num == 0)
    {
        ne.init();
        for(auto it:vacc_index)
        {
            ne.nodes[it]->strategies = 0;
            outfile<<it<<" ";
        }
        double l = Tool::getMaxEigen(ne.nodes);
        outfile<<l<<endl;
        return;
    }
    vector<int> next_index = index;             //下一个递归的可选节点
    for(int i=0;i<index.size()-num+1;i++)
    {
        int n = index[i];
        auto it = find(next_index.begin(), next_index.end(), n);
        next_index.erase(it);
        vacc_index.push_back(n);
        enumerate(outfile, ne,num-1, next_index,vacc_index);
        vacc_index.pop_back();
    }
}
int main()
{
	//used to calculate time
	clock_t start, finish;
	double totaltime;
	start = clock();
    NE ne(node_file);

    vector<int> to_vaccinate;
    for(int i=0;i<ECgame::size;i++)
        to_vaccinate.push_back(i);
    vector<int> numpy;

    int num = 6;
    stringstream ss;
    ss<<"result\\"<<num<<".txt";
    ofstream outfile(ss.str());
    enumerate(outfile, ne,num,to_vaccinate,numpy);




	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
