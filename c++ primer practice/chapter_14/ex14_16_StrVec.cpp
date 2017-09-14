#include "ex14_16_StrVec.h"

using namespace std;
void StrVec::push_back(const string &s)
{
	chk_n_alloc();          //确保有空间容纳元素
							// 在first_free指向的元素中构造s的副本
	alloc.construct(first_tree++, s);
}

pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string*e)
{
	//分配空间保存给定范围内的元素
	auto data = alloc.allocate(e - b);
	//初始化并返回一个pair，该pair由data和uninitialized_copy的返回值构成
	return { data, uninitialized_copy(b,e,data) };
}

void StrVec::free()
{
	//不能传递给deallocate一个空指针，如果ellement为0，函数什么也不做
	if (elements)
	{
		for (auto p = first_tree; p != elements;)
			alloc.destroy(--p);
		alloc.deallocate(elements, cap - elements);
	}
}

StrVec::StrVec(const StrVec &s)
{
	//调用alloc_n_copy分配空间以容纳与s中一样多的元素
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_tree = newdata.second;
	cap = newdata.second;
}

StrVec::~StrVec()
{
	free();
}

StrVec &StrVec::operator=(const StrVec &rhs)
{
	//释放原内存
	free();
	auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
	elements = newdata.first;
	first_tree = newdata.second;
	return *this;
}

void StrVec::reallocate()
{
	//我们将分配当前大小两倍的空间
	auto newcapacity = size() ? 2 * size() : 1;
	//分配新内存
	auto newdata = alloc.allocate(newcapacity);
	//将数据从旧内存移到新内存中
	auto dest = newdata;            //指向新数组中下一个空闲位置
	auto elem = elements;           //指向旧数组中下一个元素
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, move(*elem++));
	//释放旧内存空间
	free();
	elements = newdata;
	first_tree = dest;
	cap = elements + newcapacity;
}

bool operator==(const StrVec &sv1, const StrVec &sv2)
{
	return(sv1.size() == sv2.size() && equal(sv1.begin(), sv1.end(), sv2.begin()));
}
bool operator!=(const StrVec &sv1, const StrVec &sv2)
{
	return !(sv1 == sv2);
}
bool operator<(const StrVec &sv1, const StrVec &sv2)
{
	return sv1.size() < sv2.size();
}
bool operator>(const StrVec &sv1, const StrVec &sv2)
{
	return sv1.size() > sv2.size();
}