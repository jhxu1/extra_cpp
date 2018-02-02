#pragma once
#include <random>
using namespace std;

class NE;
extern const int subsidyNum;
extern default_random_engine engine;
void subsidyProcess(NE &ne, vector<int> &subsidy_index);
