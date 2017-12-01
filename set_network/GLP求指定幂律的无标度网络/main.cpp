//============================= GLP.cpp ============================//
// generate power law topology using Tian Bu's GLP model.
//
// Date: Dec. 27th, 2001.
// Input parameters:
//  m,  Ne, Nv, alpha
// Let m0 be two times of m.

// Usage:  GLP  m  alpha
//==================================================================//
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

#define Nv	5000     // node number.
#define Ne  30000     //edge number. In average, 8 edges/node

// global variable.

long EdgeData[Ne][2];  // [][0] is from; [][1] is to.
long NodeDegree[Nv + 1];
long expGenerator(double); // exponential distribution generator.
double m, alpha, p, beta;
long m0;

//------------------
void initialGraph(void);
double rand01(void);
void preferProb(double *, long);
long  randSelect(double, double *, long);


int main(long argc, char * argv[]) {
	long i, j;
	long fromNode, toNode;
	double temp, prob, fromProb, toProb, NodePreferProb[Nv + 1];
	time_t beginTime, endTime;
	long simulTime, nodeNum, edgeNum;

    m = 2;
    alpha = 2.5;
	ofstream out("result.txt");


	// calculate probability p, and beta and m0
	m0 = long(m * 10);
	p = double(Ne - m*Nv) / double(Ne);
	beta = m*(2 + alpha + alpha*p) / (1 - p);

	srand((unsigned)time(NULL));
	(void)time(&beginTime);

	initialGraph();
	nodeNum = m0;  edgeNum = m0 - 1;

	long m_1 = long(m);
	long m_2 = long(m) + 1;
	long addEdge;
	double prob_m_1 = double(m_2 - m) / double(m_2 - m_1);

	while (nodeNum < Nv) {
		prob = rand01();
		preferProb(NodePreferProb, nodeNum);
		if (rand01() < prob_m_1)
			addEdge = m_1;
		else addEdge = m_2;
		if (prob < p) {           // add m new links.
			for (i = 1; i <= addEdge; i++) {
				edgeNum++;
				fromProb = rand01(); toProb = rand01();
				fromNode = randSelect(fromProb, NodePreferProb, nodeNum);
				EdgeData[edgeNum][0] = fromNode;  NodeDegree[fromNode] ++;
				toNode = randSelect(toProb, NodePreferProb, nodeNum);
				EdgeData[edgeNum][1] = toNode;    NodeDegree[toNode]++;
			}
		}
		else {                   // add a new node and m links from it.
			nodeNum++;
			NodeDegree[nodeNum] = addEdge;
			for (i = 1; i <= addEdge; i++) {
				EdgeData[edgeNum + i][0] = nodeNum;
				toProb = rand01();
				toNode = randSelect(toProb, NodePreferProb, nodeNum - 1);
				EdgeData[edgeNum + i][1] = toNode; NodeDegree[toNode] ++;
			}
			edgeNum += addEdge;
		};
	};

	// output data into file, use the similar format as BRITE.
	out<<"Power Law graph parameters:\n";
	out << "m=" << m << "\t alpha=" << alpha << "\t Nv=" << Nv << "\t Ne=" << Ne << endl;
	out<< "beta="<<beta<<" \t  p="<<p<<" \n";
	out << "nodeNum = " << nodeNum << "\t  edgeNum=" << edgeNum << endl;
	out << "\n Nodes: " << nodeNum << endl;
	for (i = 1; i <= nodeNum; i++)
		out << i << "\t" << NodeDegree[i] << endl;
	out << "Edges:  " << edgeNum << endl;
	for (i = 1; i <= edgeNum; i++)
		out << i << "\t" << EdgeData[i][0] << "\t" << EdgeData[i][1] << endl;
	out.close();
	(void)time(&endTime);
	simulTime = (long)(endTime - beginTime);
	printf("\n Simulation finished by using  %ld seconds\n", simulTime);

	ofstream outfile("sc-free.txt");
    for (i = 1; i <= edgeNum; i++)
		outfile << EdgeData[i][0]-1 << "\t" << EdgeData[i][1]-1 << endl;
	return 0;
}

//=============================================

void initialGraph(void) {
	long i, j;
	for (i = 1; i <= Ne; i++) {
		EdgeData[i][0] = 0; EdgeData[i][1] = 0;
	}
	for (i = 1; i <= Nv; i++) {
		NodeDegree[i] = 0;
	}
	// first generate m0 nodes with m0-1 edges;
	for (i = 1; i <= m0 - 1; i++) {
		EdgeData[i][0] = i; EdgeData[i][1] = i + 1;
	}
	for (i = 2; i <= m0 - 1; i++)
		NodeDegree[i] = 2;
	NodeDegree[1] = 1; NodeDegree[m0] = 1;
}

//---------------------------------------------------------------
double rand01(void) {
	return (double(rand()) + 0.5) / ((double)RAND_MAX + 1);
}

long expGenerator(double rate) { // exponential distribution generator.
	return long(floor(0.5 - log(1 - rand01()) / rate));
}
//----------------------------------------------------------------
void preferProb(double NodePreferProb[], long nodeNum) {
	long i;
	double degreeSum = 0;
	for (i = 1; i <= nodeNum; i++)
		degreeSum += (NodeDegree[i] - beta);
	for (i = 1; i <= nodeNum; i++)
		NodePreferProb[i] = (NodeDegree[i] - beta) / degreeSum;
}
//-----------------------------------------------------------------
long randSelect(double prob, double NodePreferProb[], long nodeNum) {
	double sum = 0;
	for (long i = 1; i <= nodeNum; i++) {
		sum += NodePreferProb[i];
		if (prob <= sum)
			return long(i);
	};
	printf("something wrong in randSelect function.\n");
	return nodeNum;
}

