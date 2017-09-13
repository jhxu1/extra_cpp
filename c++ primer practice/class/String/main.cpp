#include <allocators>
#include <memory>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

using namespace std;
class String
{
	friend ostream& operator<<(ostream &os, const String s);
public:
	String() :begin(nullptr), end(nullptr) {}
	String(const char *s)
	{
		char* s1 = const_cast<char*>(s);
		while (*s1) ++s1;
		auto newdata = alloc_n_copy(s, s1);
		begin = newdata.first;
		end = newdata.second;
	}
	String(const String &s)		//拷贝构造
	{
		auto newdata = alloc_n_copy(s.begin, s.end);
		begin = newdata.first;
		end = newdata.second;
	}
	String &operator=(const String &s)
	{
		free();
		auto newdata = alloc_n_copy(s.begin, s.end);
		begin = newdata.first;
		end = newdata.second;
		return *this;
	}
	~String()
	{
		free();
	}
	void free()//释放内存  
	{
		if (begin)
		{
			for_each(begin, end, [this](char &rhs) {alloc.destroy(&rhs); });
			alloc.deallocate(begin, end - begin);
		}
	}
private:
	allocator<char> alloc;											//分配元素
	pair<char*, char*> alloc_n_copy(const char*b, const char*e)
	{
		//分配空间保存给定范围内的元素
		auto data = alloc.allocate(e - b);
		//初始化并返回一个pair，该pair由data和uninitialized_copy的返回值构成
		return { data, uninitialized_copy(b,e,data) };
	}
	char *begin;
	char *end;
};

ostream &operator << (ostream &os, const String s)
{
	for (auto i = s.begin; i != s.end; i++)
		os << *i;
	return os;
}

int main()
{
	String s1("ddd");
	cout << s1 << endl;
	system("pause");
	return 0;
}