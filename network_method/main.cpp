#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdexcept>
#include <time.h>
#include "main.h"
#include "RNG.h"

using namespace std;
const int SIZE = 1000;

template <typename T>
void Shuffe(vector<T> &a, int x, int y)
{
    int n = a.size();
    if(y >= n || x > y)
    {
        cout<<"x: "<<x<<" y:  "<<y<<"  size:  "<<n<<endl;
        throw runtime_error("wrong");
    }

    default_random_engine engine(time(nullptr));            //不加此语句的话，每次随机的数会不变
    for (int i = y; i > x; --i)
    {
        uniform_int_distribution<> dis(x, i-1);
        swap(a[i], a[dis(engine)]);
    }
}

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
    int size = nodes.size();
    sort(nodes.begin(),nodes.end(), [](Node &n1, Node &n2){return n1.degree>n2.degree;});
    for(int i=0;i<size;)
    {
        cout<<"--------洗扑克-------"<<endl;
        int value = nodes[i].degree;
        int j=i+1;
        while(j<size && nodes[j].degree == value)
            j++;
        Shuffe(nodes, i, j-1);
        i = j;
    }
    ofstream outfile("degree_sort.txt");
    for(auto i:nodes)
        outfile<<i.flag<<endl;
}

void k_shell(vector<Node> nodes)
{
    ofstream outfile("kshell_sort.txt");
    vector<int> kshell_sort;
    int size = nodes.size();
    vector<int> k_degree(size,-100);
    vector<int> temp_degree(size,-100);
    for(auto i:nodes)
    {
        int flag = i.flag;
        temp_degree[flag] = i.degree;
    }
    int ks = 0;
    while(true)
    {
        bool flag = true;
        while(flag)
        {
            vector<int> ks_vec;
            for(int i=0;i<size;i++)
            {
                if(temp_degree[i] == ks && k_degree[i]<0)
                {
                    cout<<"ks:  "<<ks<<"  index:  "<<i<<endl;
                    kshell_sort.push_back(i);
                    ks_vec.push_back(i);
                    k_degree[i] = ks;
                }
            }
            if(ks_vec.empty())
                flag = false;
            for(auto it:ks_vec)
            {
                for(auto it2:nodes[it].neighbours)
                    temp_degree[it2] -- ;
            }

        }
        ks++;
        bool ifAll = false;
        for(auto i:k_degree)
        {
            if(i<0)
            {
                ifAll = true;
                break;
            }
        }
        if(ifAll == false)
            break;
    }
    reverse(kshell_sort.begin(),kshell_sort.end());
    for(auto i:kshell_sort)
        outfile<<i<<endl;
}


int main()
{
    vector<Node> nodes = load_net("1000sc-free.txt");
    degree_sort(nodes);
    k_shell(nodes);
    system("pause");
}
