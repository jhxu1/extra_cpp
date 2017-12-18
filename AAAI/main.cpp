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

using namespace std;

const string node_file = "sc-free.txt";
const float final_point = 1;
const float first_point = 0.2;

float frac = 0.2;
const int subsidyNum = 20;

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


int main()
{
	//used to calculate time
	clock_t start, finish;
	double totaltime;
	start = clock();

	//处理节点
	NE ne(node_file);

	ofstream outfile("degree.csv");
	Tool::outdegree(outfile, ne.nodes);
	outfile.close();

	outfile.open("vacc_info.txt");
    vector<int> EI_sort = load_EI_sort();
    vector<int> degree_sort = load_degree_sort(ne);
    vector<Node*> subsidyNode;
    for(int i=0;i<subsidyNum;i++)
    {
        int index = EI_sort[i];
        //int index = degree_sort[i];
        cout<<index<<endl;
        subsidyNode.push_back(ne.nodes[index]);
    }
	while(frac<1)
    {
        ne.init();
         double t = ne.getLambda1()*frac;
         cout<<"****threshold: "<<t<<"  ****"<<endl;
         ne.subsidy(subsidyNode);
         outfile<<t<<"\t"<<ne.LDG(t)<<endl;
         frac+=0.1;
    }



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
