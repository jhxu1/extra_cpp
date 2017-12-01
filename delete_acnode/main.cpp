#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
class Node
{
public:
    Node():degree(0){}
    int index;
    int degree;
    vector<Node*> nei;
};

int main()
{
	ifstream infile("oregon-1.txt");
    vector<int> sta;
    vector<int> end;
    int a,b;int maxNum = 0;
    while(infile>>a>>b)
    {
        sta.push_back(a);
        end.push_back(b);
        if(a>maxNum)    maxNum =a;
        if(b>maxNum)    maxNum = b;
    }
    vector<Node*> nodes(maxNum+1);
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i] = new Node;
		nodes[i]->index = i;
	}
    for(int i=0;i<sta.size();i++)
    {
        int a = sta[i],b = end[i];
		if (b > a)
		{
			nodes[a]->nei.push_back(nodes[b]);
		}
        else
            nodes[b]->nei.push_back(nodes[a]);
        nodes[a]->degree++;
        nodes[b]->degree++;
    }
	auto it = nodes.begin();
	int flag = 0;
    while(it != nodes.end())
    {
        cout<<flag++<<endl;
        if((*it)->degree == 0)
        {
            auto it2 = it;
            while(it2!=nodes.end())
            {
                (*it2)->index --;
                it2++;
            }
            it = nodes.erase(it);
        }
		else
		{
			it++;
		}
    }
    ofstream outfile("result.txt");
    for(int i=0;i<nodes.size();i++)
    {
        int a = nodes[i]->index;
        for(int j=0;j<nodes[i]->nei.size();j++)
        {
            int b = nodes[i]->nei[j]->index;
            if(b>a)
                outfile<<a<<"\t"<<b<<endl;
        }
    }
    system("pause");
}
