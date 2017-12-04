#include <stdlib.h>
#include <random>
#include <vector>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;
class Node
{
	friend void set_wellmixed_net();
public:
	Node(int f) :flag(f), strategies(1) {}
	Node() = default;
	~Node() = default;
	size_t degree() { return neighbours.size(); }
	std::vector<Node*> nei() { return neighbours; }
	int getFlag() { return flag; }
	int strategies;				// 0(接种） 1（不接种）
private:
	std::vector<Node*> neighbours;
	int flag;
};

const int SIZE = 15;
const double C = 0.1;             //接种花费
const double L = 0;             //未接种未染病花费
const double Le = 1;            //染病花费
const float learning_rate = 0.2;	//RLA算法学习率
const float TAO = 2.0/3;
vector<Node*> nodes;

void set_wellmixed_net()
{
	for (int i = 0; i < SIZE; i++)
		nodes.push_back(new Node(i));
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE;j++)
		{
			if (j != i)
			{
				nodes[i]->neighbours.push_back(nodes[j]);
			}
		}
	}
}

void deleteNode()
{
	for (auto i : nodes)
	{
		delete(i);
		i = nullptr;
	}

}

void RLA_algorithm(double T)
{
	//1) Set an initial probability pi(0), for each user i =1,...,N.
	size_t size = nodes.size();
	vector<double> pi(size, 0.5);
	vector<double> payoff(size);
	default_random_engine engine(time(nullptr));		//随机数种子
	uniform_real_distribution<double> u(0, 1);
	ofstream outfile("result.txt");
	ofstream outfile1("nodes_vacc_pro.csv");
	int last_vacc_num = 0;
	int times=0;
	while (true)
	{
		int vacc_num = 0;
		for (int i = 0; i < size; i++)
		{
			// 2) At every time step t, each node i invests with probability
			// pi(t).This determines his pure action σi(t).
			float ran = u(engine);
			if (ran > pi[i])
				nodes[i]->strategies = 1;
			else
			{
				nodes[i]->strategies = 0;
				vacc_num++;
			}

		}
		// 3) Each player i obtains his payoff Si(σi(t); σ ¯(t))
		for (int i = 0; i < size; i++)
		{
			if (nodes[i]->strategies == 0)
				payoff[i] = C;
			else
			{
			    int non_vacc_num = size-vacc_num;
				if (non_vacc_num>=2 && T >= 1.0 / (non_vacc_num - 1))
                {
                    payoff[i] = Le * (1 - 1.0 / (T* (non_vacc_num-1  )));
                }
				else
					payoff[i] = 0;
			}
			payoff[i] =payoff[i]/( C + Le);
		}
		// 4) Each node i updates its probability according to the following rule :
		for (int i = 0; i < size; i++)
		{
			pi[i] = pi[i] + learning_rate * (1-payoff[i]) * (1 - nodes[i]->strategies - pi[i]);
		}
		times++;
		cout <<times<<"\t"<< pi[0] << "\t"<<payoff[0]<<"\t"<<nodes[0]->strategies<<endl;
		outfile << times << "\t" << vacc_num << endl;
		outfile1<<times<<",";
		for(int i=0;i<size;i++)
            outfile1<<pi[i]<<",";
        outfile1<<endl;


        bool flag = true;
        for(int i=0;i<size;i++)
        {
            if(pi[i] > 0.01 && pi[i] < 1-0.01)
            {
                flag = false;
                break;
            }
        }
        if(flag == true)
        {
            cout<<"non-vacc_num:   "  <<size-vacc_num<<endl;
            break;
        }

	}
}

int main()
{
	set_wellmixed_net();
	RLA_algorithm(TAO);
	system("pause");
}
