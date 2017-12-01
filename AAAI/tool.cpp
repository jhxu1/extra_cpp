#include "tool.h"




using namespace std;



void Tool::mult(const matrix &A,vector<double> &X,vector<double> &Y,size_t N)
{
	for (size_t row = 0; row<N; row++)
	{
		X[row] = 0;
		for (size_t col = 0; col<N; col++)
			X[row] += A[row][col] * Y[col];
	}
}

void Tool::to1(vector<double> &X,vector<double> &Y,double &maxl,size_t N)
{
	long double tmp = fabs(X[0]);
	for (size_t i = 1; i<N; i++)
	{
		if (fabs(X[i])>tmp)
			tmp = fabs(X[i]);
	}
	for (size_t i = 0; i<N; i++)
	{
		if (tmp != 0)
			Y[i] = X[i] / tmp;
		else
			Y[i] = 0;
	}
	maxl = tmp;
}

double Tool::getMaxEigen(const vector<Node*> nodes)
{
	matrix A;
	toMatrix(A,nodes);
	size_t N = A.size();

	//----ceshi
	//size_t N = 3;
	//A = { {0,0,0},{0,1.7,0},{0,0,-1.7} };
	//=---


	vector<double> Y(N, 1);
	vector<double> X(N, 0);
	double maxl = 0;
	double ago = maxl + 100.0;
	vector<double> feature_vector(N);
	int k = 1;
	double m1=0, m2=0;
	while (fabs(maxl - ago)>0.01 || k<3)
	{
		ago = maxl;
		m1 = m2;
		feature_vector = Y;
		mult(A,X,Y,N);		//矩阵相乘:X=AY
		to1(X,Y,m2,N);		//找出X中的最大值，归一化X保存到Y
		maxl = sqrt(m1*m2);
		k++;
		if(k==1000)
            return -1;
	}
	std::cout  << "totally run " << k - 1 << " times\t";
	//cout << "maximum eigenvalue ---->\t" <<maxl << endl;
	return maxl;
}

//double Tool::getMaxEigen2(const vector<Node*> nodes)
//{
//	size_t N = nodes.size();
//    MatrixXd A(N, N);
//
//	for (int i = 0; i < N; i++)
//	{
//		for (int j = i; j < N; j++)
//		{
//			if (nodes[i]->ifNei(nodes[j]))
//			{
//				A(i, j) = 1;
//				A(j, i) = 1;
//			}
//			else
//			{
//				A(i, j) = 0;
//				A(j, i) = 0;
//			}
//		}
//	}
//	EigenSolver<MatrixXd> es(A);
//	MatrixXcd evals = es.eigenvalues();
//	MatrixXd evalsReal = evals.real();
//	double result = -1000;
//	for (int i = 0; i < N; i++)
//	{
//		if (evalsReal(i) > result)
//			result = evalsReal(i);
//	}
//	cout << "最大特征值:\t"<<result << endl;
//	return result;
//}

void Tool::outdegree(ostream &out, vector<Node*> &nodes)
{
	size_t maxdegree = 0;
	for (auto p : nodes)
	{
		if (p->degree() > maxdegree)
			maxdegree = p->degree();
	}
	vector<size_t> degree(maxdegree + 1);
	for (auto p : nodes)
	{
		degree[p->degree()]++;
	}
	for (int i=0;i<degree.size();i++)
    {
        if(degree[i]!=0)
            out << i<<","<<degree[i] << endl;
    }
	return;
}
