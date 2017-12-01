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

const string node_file = "oregan.txt";
const float final_point = 1;
const float first_point = 0.2;

float frac = 0.3;

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

	outfile.open("mincost.txt");
	while(frac<0.33)
    {
         double t = ne.getLambda1()*frac;
         cout<<"****threshold: "<<t<<"  ****"<<endl;
         //ne.subsidy(2);
         outfile<<t<<"\t"<<ne.HDG(t)<<endl;
         frac+=0.1;
    }


	vector<double> result;


	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
