#include "Node.h"

class Node;
std::vector<Node*> inputData(const std::string &filename)
{
	std::vector<Node*> nodes;
	for (int i = 0; i < ECgame::size; i++)
	{
		Node* n = new Node(i);
		nodes.push_back(n);
	}
	std::ifstream infile(filename);
	int a, b;
	while (infile >> a >> b)
	{
		nodes[a]->neighbours.push_back(nodes[b]);
		nodes[b]->neighbours.push_back(nodes[a]);
	}
	return nodes;
}

void deleteNode(std::vector<Node*> nodes)
{
	for (auto i : nodes)
		delete(i);
}

void toMatrix(std::vector<std::vector<double>> &matrix, const std::vector<Node*> &nodes)
{
    vector<Node*> temp = nodes;
    for(auto it = temp.begin();it!=temp.end();)
    {
        if((*it)->strategies == 0)
            it = temp.erase(it);
        else
            it++;
    }
    int size = temp.size();
	int size2 = count_if(nodes.begin(), nodes.end(), [](Node *n) {return n->strategies == 1; });
	for (size_t i=0;i<size;i++)
		matrix.push_back(std::vector<double>(size, 0));
	for (int i = 0; i < nodes.size(); i++)
	{
	    //ÒÆ³ý½Úµã±àºÅ
		if (nodes[i]->strategies == 0 && nodes[i]->flag != -1)
		{
		    cout<<"something wrong in toMatrix"<<endl;
			for (int j = 0; j < nodes.size(); j++)
			{
				if (nodes[j]->flag > nodes[i]->flag)
				{
					nodes[j]->flag--;
				}
			}
			nodes[i]->flag = -1;
		}
	}
	for(int i=0;i<size;i++)
    {
        if(temp[i]->strategies == 1)
        {
            for(int j=i+1;j<size;j++)
            {
				if (temp[j]->strategies == 1 && (temp[j]->ifNei(temp [i])))
                {
                    matrix[i][j] = 1;
                    matrix[j][i] = 1;
                }
            }
        }
    }
	//for (int i = 0; i < nodes.size(); i++)
	//{
	//	if (nodes[i]->strategies == 1)
	//	{
	//		for (auto j : nodes[i]->nei())
	//		{
	//			if (j->strategies == 1 && j->flag> nodes[i]->flag)
	//			{
	//				int index0 = nodes[i]->flag;
	//				int index = j->flag;
	//				matrix[index0][index] = 1;
	//				matrix[index][index0] = 1;
	//			}
	//		}
	//	}
	//}
}

bool operator==(const Node &n1, const Node &n2)
{
	return n1.flag == n2.flag;
}

bool Node::ifNei(Node *n1)
{
	for (auto p : n1->nei())
	{
		if (*p == *this)
			return true;
	}
	return false;
}
