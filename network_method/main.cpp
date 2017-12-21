#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "main.h"
#include "RNG.h"

using namespace std;
const int SIZE = 15;



vector<Node> load_net(const string &fileName)
{
    vector<Node> nodes;
    for(int i=0;i<SIZE;i++)
    {
        nodes.push_back(Node(i));
    }
    ifstream infile(fileName);
    int a,b;
    while(infile>>a>>b)
    {
        nodes[a].degree++;nodes[b].degree++;
        nodes[a].neighbours.push_back(b);
        nodes[b].neighbours.push_back(a);
    }
    return nodes;
}

void degree_sort(vector<Node> nodes)
{
    sort(nodes.begin(),nodes.end(), [](Node &n1, Node &n2){return n1.degree>n2.degree;});
    ofstream outfile("degree_sort.txt");
    for(auto i:nodes)
        outfile<<i.flag<<endl;
}

void k_shell(vector<Node> nodes)
{
    int size = nodes.size();
    vector<bool> flag(size, false);
    vector<int> k_shell_sort;
    for(int i=0;i<size;i++)
    {
        int min_degree = 1000000;
        int index = -1;
        for(int j=0;j<size;j++)
        {
            if(flag[j] == false && nodes[j].degree < min_degree)
            {
                min_degree = nodes[j].degree;
                index = j;
            }
        }
        cout<<index<<endl;
        k_shell_sort.push_back(index);
        nodes[index].degree = -1;flag[index] = true;
        for(auto i:nodes[index].neighbours)
            nodes[i].degree--;
    }
    ofstream outfile("kshell_sort.txt");
    for(auto it = k_shell_sort.rbegin();it!=k_shell_sort.rend();it++)
        outfile<<*it<<endl;
}


int main()
{
    vector<Node> nodes = load_net("random.txt");
    degree_sort(nodes);
    k_shell(nodes);
    system("pause");
}
