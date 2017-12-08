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

const string node_file = "random.txt";
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


void find_minNE(ostream &outfile, double l)
{
    int vacc_num = 0;
    int flag = 0;
    ifstream infile;
    for(int num =0; num<ECgame::size;num++)
    {
        if(flag == 1)
            break;
        stringstream ss;
        ss<<"result\\"<<num<<".txt";
        infile.open(ss.str());
        string s;
        while(getline(infile, s))
        {
            ss.clear(); ss.str(s);
            vector<double> data; double temp;
            while(ss>>temp)
                data.push_back(temp);
            double lam = *data.rbegin();
            if(lam < l)
            {
                vacc_num = num;
                outfile<<l<<" "<<num<<" ";
//                for(auto it:data)
//                    outfile<<(int)it<<" ";
                outfile<<endl;
                flag = 1;
                break;
            }
        }
        infile.close();
    }
}

void find_maxNE(ostream &outfile, double l, NE &ne)
{
    int vacc_num = 0;
    int flag = 0;
    ifstream infile;
    for(int num =ECgame::size-1; num>=0;num--)
    {
        if(flag == 1)
            break;
        stringstream ss;
        ss<<"result\\"<<num<<".txt";
        infile.open(ss.str());
        string s;
        while(getline(infile, s))
        {
            ss.clear(); ss.str(s);
            vector<double> data; double temp;
            while(ss>>temp)
                data.push_back(temp);
            double lam = *data.rbegin();
            data.pop_back();
            if(lam < l)
            {
                ne.init();
                for(auto i:data)
                    ne.nodes[i]->strategies = 0;
                int flag2 = 1;
                for(auto i:data)
                {
                    ne.nodes[i]->strategies = 1;
                    double temp_l = Tool::getMaxEigen(ne.nodes);
                    if(temp_l < l)
                    {
                        flag2 = 0;
                        break;
                    }
                    ne.nodes[i]->strategies = 0;
                }
                if(flag2 == 1)
                {
                    vacc_num = num;
                    outfile<<l<<" "<<num<<" ";
                    for(auto it:data)
                        outfile<<(int)it<<" ";
                    outfile<<endl;
                    flag = 1;
                    break;
                }
            }
        }
        infile.close();
    }
}

int main()
{
	//used to calculate time
	clock_t start, finish;
	double totaltime;
	start = clock();
    NE ne(node_file);

//    vector<int> to_vaccinate;
//    for(int i=0;i<ECgame::size;i++)
//        to_vaccinate.push_back(i);
//    vector<int> numpy;
//
//    for(int num = 0;num<ECgame::size;num++)
//    {
//        stringstream ss;
//        ss<<"result\\"<<num<<".txt";
//        ofstream outfile(ss.str());
//        enumerate(outfile, ne,num,to_vaccinate,numpy);
//    }

    ofstream outfile("max NE.txt");
    while(frac<1)
    {
        double l = ne.lambda1 * frac;
        find_maxNE(outfile, l, ne);
        frac+=0.1;
    }




	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
