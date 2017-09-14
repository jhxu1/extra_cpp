#include "ex14_16_StrBlob.h"

StrBlob::StrBlob(initializer_list<string> il)
{
	data = make_shared<vector<string>>(vector<string>{il});
}
StrBlob::StrBlob(string il)
{
	data = make_shared<vector<string>>(vector<string>{il});
}

void StrBlob::check(size_type i, const string &msg) const
{
	if (i >= data->size())
		throw out_of_range(msg);
}

void StrBlob::pop_back()
{
	check(0, "pop_back on empty StrBlob");
	data->pop_back();
}
string& StrBlob::front() const
{
	check(0, "pop_back on empty StrBlob");
	return data->front();
}
string& StrBlob::back() const
{
	check(0, "pop_back on empty StrBlob");
	return data->back();
}

StrBlobPtr StrBlob::begin()
{
	return StrBlobPtr(*this);
}
StrBlobPtr StrBlob::end()
{
	return StrBlobPtr(*this, data->size());
}

//==运算符
bool operator==(const StrBlob &s1, const StrBlob &s2)
{
	return *s1.data == *s2.data;
}
bool operator!=(const StrBlob &s1, const StrBlob &s2)
{
	return !(s1 == s2);
}
bool operator==(const StrBlobPtr &s1, const StrBlobPtr &s2)
{
	return s1.curr == s2.curr;
}
bool operator!=(const StrBlobPtr &s1, const StrBlobPtr &s2)
{
	return !(s1 == s2);
}

//关系运算符
bool operator<(const StrBlob &s1, const StrBlob &s2)
{
	return s1.size() < s2.size();
}
bool operator>(const StrBlob &s1, const StrBlob &s2)
{
	return s1.size() > s2.size();
}
bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2)
{
	return s1.curr > s2.curr;
}
bool operator<(const StrBlobPtr &s1, const StrBlobPtr &s2)
{
	return s1.curr < s2.curr;
}


