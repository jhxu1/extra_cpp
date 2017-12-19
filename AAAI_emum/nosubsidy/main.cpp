#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <windows.h>
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
功能：枚举num个人接种的情况，输出在result文件夹中
*/
void enumerate(ostream &outfile, NE &ne, int num,  vector<int> index, vector<int> vacc_index)
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
        enumerate(outfile, ne,num-1 , next_index,vacc_index);
        vacc_index.pop_back();
    }
}


/*
功能：寻找最小NE花费
输入：outfile（输出流）l（阈值T）
*/
vector<double> find_minNE(ostream &outfile, vector<int> sub_index, double l)
{
    int vacc_num = 0;
    int flag = 0;
    ifstream infile;
    vector<double> result;
    for(int num =0; num<ECgame::size-sub_index.size();num++)
    {
        if(flag == 1)
            break;
        stringstream ss;
        ss<<"result\\index_";
        ss<<sub_index[0]<<","<<sub_index[1]<<"\\";
        ss<<num<<".txt";
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
                vacc_num = num+sub_index.size();
                outfile<<l<<" "<<vacc_num<<" ";
                result.push_back(l);
                result.push_back(vacc_num);
                for(auto it:data)
                {
                    outfile<<(int)it<<" ";
                    result.push_back(it);
                }
                outfile<<endl;
                flag = 1;
                break;
            }
        }
        infile.close();
    }
    return result;
}

/*
功能：寻找最大NE花费
输入：outfile（输出流）l（阈值T）
*/
vector<double> find_maxNE(ostream &outfile, double l, NE &ne)
{
    int vacc_num = 0;
    int flag = 0;
    vector<double> result;
    ifstream infile;
    for(int num =ECgame::size-1; num>=0;num--)
    {
        if(flag == 1)
            break;
        stringstream ss;

        ss<<"result\\index_NULL\\";
        ss<<num<<".txt";
        infile.open(ss.str());
        string s;
        while(getline(infile, s))
        {
            ss.clear(); ss.str(s);
            vector<double> data; double temp;
            while(ss>>temp)
                data.push_back(temp);
            double lam = *data.rbegin();
            data.pop_back();            //移除lambda
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
                    outfile<<l<<" "<<vacc_num<<" ";
                    result.push_back(l);result.push_back(vacc_num);
                    for(auto it:data)
                    {
                        outfile<<(int)it<<" ";
                        result.push_back(it);
                    }
                    outfile<<endl;
                    flag = 1;
                    break;
                }
            }
        }
        infile.close();
    }
    return result;
}


vector<double> findmin(vector<vector<double>> V)
{
    double min = 10000;
    vector<double> result;
    for(auto vec:V)
    {
        cout<<vec.size()<<endl;
        if(vec[1] < min)
        {
            min = vec[1];
            result = vec;
        }
    }
    return result;
}
vector<double> findmax(vector<vector<double>> V)
{
    double max = -10000;
    vector<double> result;
    for(auto vec:V)
    {
        cout<<vec.size()<<endl;
        if(vec[1] > max)
        {
            max = vec[1];
            result = vec;
        }
    }
    return result;
}

void Nosubsidy(NE &ne)
{
    vector<int>sub_index;
    sub_index.push_back(-1);sub_index.push_back(-1);
    vector<int> to_vaccinate;
    for(int k=0;k<ECgame::size;k++)
    {
        to_vaccinate.push_back(k);
    }
    vector<int> numpy;
    for(int num = 0;num<ECgame::size;num++)
    {
        stringstream ss;
        ss<<"result\\index_NULL";
        CreateDirectory(ss.str().c_str(), NULL);
        ss<<"\\"<<num<<".txt";
        cout<<ss.str()<<endl;
        ofstream outfile(ss.str());
        enumerate(outfile, ne,num, to_vaccinate,numpy);
    }
    ofstream outfile("max NE.txt");
    ofstream outfile1("max data.txt");
    while(frac<1)
    {
        double l = ne.lambda1 * frac;
        vector<double> result;
        result=find_maxNE(outfile1, l, ne);
        for(auto it:result)
        {
            cout<<it<<" ";
            outfile<<it<<" ";
        }

        outfile<<endl;
        frac+=0.1;
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
    for(int k=0;k<ECgame::size;k++)
    {
        to_vaccinate.push_back(k);
    }
    vector<int> numpy;
//    for(int num = 0;num<ECgame::size;num++)
//    {
//        stringstream ss;
//        ss<<"result\\index_NULL";
//        CreateDirectory(ss.str().c_str(), NULL);
//        ss<<"\\"<<num<<".txt";
//        cout<<ss.str()<<endl;
//        ofstream outfile(ss.str());
//        enumerate(outfile, ne,num, to_vaccinate,numpy);
//    }
    ofstream outfile("max NE.txt");
    ofstream outfile1("max data.txt");
    while(frac<1)
    {
        double l = ne.lambda1 * frac;
        vector<double> result;
        result=find_maxNE(outfile1, l, ne);
        //result=find_minNE(outfile1, l);
        for(auto it:result)
        {
            cout<<it<<" ";
            outfile<<it<<" ";
        }
        cout<<endl;
        outfile<<endl;
        frac+=0.1;
    }




	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
