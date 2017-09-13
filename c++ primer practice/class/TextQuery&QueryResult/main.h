#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#include <string>
#include <iostream>
#include <cctype>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;
class QueryResult;
class TextQuery
{
public:
	TextQuery(ifstream&);
	QueryResult query(string &s);
	vector<string> m_text;                      //用于存放文本的每一行内容
	map<string, set<size_t>> m_line;            //set中存放每个单词的行号，用map进行关联
};

class QueryResult {
	friend ostream& print(ostream&, const QueryResult &);
public:
	QueryResult(const string &word, shared_ptr<set<size_t>> &row, shared_ptr<vector<string>> &data)
		:m_word(word), m_row(row), m_data(data) {}
private:
	string m_word;
	shared_ptr<set<size_t>> m_row;
	shared_ptr<vector<string>> m_data;
	set<size_t>::iterator begin() const { return m_row->begin(); }
	set<size_t>::iterator end() const { return m_row->end(); }
	shared_ptr<vector<string>> get_file() const { return m_data; }
};

void runQueries(ifstream &infile);


#endif // MAIN_H_INCLUDED