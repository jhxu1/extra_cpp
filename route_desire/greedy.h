#ifndef GREEDY_H_INCLUDED
#define GREEDY_H_INCLUDED

deque<size_t> Greedy(vector<CNode> nodeVec, vector<CLink> linkVec);
double GetCost(vector<CNode> &nodeVec,vector<CLink> temp_linkVec, deque<size_t> greedyResult);

#endif // GREEDY_H_INCLUDED
