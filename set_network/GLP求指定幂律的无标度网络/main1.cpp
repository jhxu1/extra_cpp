#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;
using namespace std::placeholders;

istream& read(istream &infile)
{
    string word;
    while(infile>>word)
    {
        cout<<word<<endl;
    }
    infile.clear();
    return infile;
}
void cout_list(forward_list<int> flst)
{
    for(auto it = flst.begin();it!=flst.end();it++)
        cout<<*it<<endl;
}
bool isShorter(const string &s1, const string &s2)
{
    return s1.size()<s2.size();
}
void elimDups(vector<string> &words)
{
    //≈≈–Ú
    sort(words.begin(), words.end(), [](const string &s1, const string &s2){return s1.size()<s2.size();});
    //»•÷ÿ∏¥
    auto it = unique(words.begin(), words.end());
    words.erase(it, words.end());
}
void cout_list(vector<int> flst)
{
    for(auto it = flst.begin();it!=flst.end();it++)
        cout<<*it<<endl;
}
void cout_list(vector<string> flst)
{
    for(auto it = flst.begin();it!=flst.end();it++)
        cout<<*it<<endl;
}
bool short6(const string &s1, string::size_type sz)
{
    return s1.size()<sz;
}
bool check_size(const string &s1, string::size_type sz)
{
    return s1.size()>sz;
}
int main()
{
//    istream_iterator<int> in_iter(cin);
//    istream_iterator<int> int_eof;
//    vector<int> vec(in_iter,int_eof);

    list<int> lst = {0,1,2,3,4,5,6,7,8,9};
    vector<int> vec(lst.begin(), lst.end());
    //sort
    ostream_iterator<int> out_iter(cout, "\n");
    sort(vec.begin(), vec.end());
    unique_copy(vec.cbegin(), vec.cend(), out_iter);
    system("pause");
    return 0;
}
