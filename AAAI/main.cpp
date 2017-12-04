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

int main()
{
	//used to calculate time
	clock_t start, finish;
	double totaltime;
	start = clock();

	//处理节点
	NE ne(node_file);
//	ofstream out("node.txt");
//	for(auto i:ne.nodes)
//    {
//        out<<i->getFlag()<<"\t"<<i->degree()<<endl;
//    }
	ofstream outfile("degree.csv");
	Tool::outdegree(outfile, ne.nodes);
	outfile.close();

	outfile.open("vacc_info.txt");
//	double t = ne.getLambda1()*frac;
//	cout<<"****threshold: "<<t<<"  ****"<<endl;
//	outfile<<"threshold\t"<<t<<endl;
//	outfile<<"no subsidy\t"<<ne.HDG(t)<<"\t";
////	for(auto info:ne.vacc_info)
////        outfile<<info<<",";
////    outfile<<endl;
////	for(int i=0;i<ECgame::size;i++)
////    {
////        for(int j=i+1;j<ECgame::size;j++)
////        {
////			ne.init();
////            vector<Node*> subsidyNode;
////            subsidyNode.push_back(ne.nodes[i]);
////            subsidyNode.push_back(ne.nodes[j]);
////            ne.subsidy(subsidyNode);
////            outfile<<ne.nodes[i]->getFlag()<<","<<ne.nodes[j]->getFlag()<<"\t"<<ne.HDG(t)+2<<"\t";
////            for(auto info:ne.vacc_info)
////                outfile<<info<<",";
////            outfile<<endl;
////        }
////    }
    vector<Node*> subsidyNode;
    subsidyNode.push_back(ne.nodes[0]);
    subsidyNode.push_back(ne.nodes[33]);
	while(frac<1)
    {
         double t = ne.getLambda1()*frac;
         cout<<"****threshold: "<<t<<"  ****"<<endl;
         ne.subsidy(subsidyNode);
         outfile<<t<<"\t"<<ne.HDG(t)+2<<endl;
         frac+=0.1;
    }



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
