#include "ex14_16_StrVec.h"

using namespace std;
int main()
{
	StrVec s1,s2;
	s1.push_back("aaa");
	s2.push_back("bbb");
	cout << (s1 == s2) << endl;
	system("pause");
	return 0;
}