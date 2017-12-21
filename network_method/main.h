#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

using namespace std;


class Node
{
public:
    Node(int d):flag(d){degree = 0;}
    Node(){}
    int degree;
    int flag;
    vector<int> neighbours;
};



#endif // MAIN_H_INCLUDED
