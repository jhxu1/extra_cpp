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
const float first_point = 0.5;

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

    vector<Node*> subsidyNode;

    ne.RLA_algorithm(1.3458);



	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "total time:\t" << totaltime << endl;
	system("pause");
}
