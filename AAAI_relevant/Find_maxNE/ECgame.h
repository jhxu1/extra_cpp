#pragma once
#include <algorithm>
#include <vector>
#include <random>
#include <time.h>
#include "Node.h"
#include "tool.h"

class Node;
void deleteNode(std::vector<Node*>);
/*
Epidemic Containment game
*/
class ECgame
{
	friend class Node;
	friend std::vector<Node*> inputData(const std::string&);
public:
	ECgame(const std::string &filename) { readData(filename); }
	virtual ~ECgame() { std::cout << "~EC" << std::endl; deleteNode(nodes); }
	double getLambda1() { return lambda1; }
	static int getSize() { return size; }
	const static double C;
	const static double L;
	const static double Le;			//L<C<Le
	const static float learning_rate;
	static double T;			//T = recoveryRate/transmissionRate
	std::vector<Node*> nodes;
	const static int size;
	void readData(const std::string &filename);
	double lambda1;				//max eigen value
};


/*
Nash Equilibrium
*/
class NE :public ECgame
{
	friend class Node;
public:
	NE(std::string filename):ECgame(filename){}
	~NE(){}
	double HDG(double, ofstream &outfile);		//eigenvalue: the eigenValue of HDG produced in the process
	double LDG(double t, ofstream &outfile);
	double get_NEcost(double t, const string &fileName, const string &flag, ofstream &outfile);
	void RLA_algorithm(double t);
	void restrain_flag();
	void subsidy(std::vector<Node*> pai);
	void init();
	std::vector<int> vacc_info;
private:
	double iterative_secure(double t,std::vector<Node*> &pai, ofstream &outfile);
    void remove_flag(int index, std::vector<Node*> &pai);
    void add_flag(int index, int flag, std::vector<Node*> &pai);
};
