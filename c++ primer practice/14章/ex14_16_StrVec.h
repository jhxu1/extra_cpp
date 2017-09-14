#include <allocators>
#include <memory>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
// 类vector类内存分配策略的简化实现
class StrVec
{
	friend bool operator==(const StrVec &sv1, const StrVec &sv2);
	friend bool operator!=(const StrVec &sv1, const StrVec &sv2);
public:
	StrVec() :elements(nullptr), first_tree(nullptr), cap(nullptr) {}        //默认初始化
	StrVec(const StrVec&);                                                  //拷贝构造函数
	~StrVec();                                                              //析构函数
	StrVec &operator=(const StrVec&);                                       //拷贝赋值运算符
	void push_back(const string&);                                          //拷贝元素
	size_t size() const { return first_tree - elements; }
	size_t capacity() const { return cap - elements; }
	string *begin() const { return elements; }
	string *end() const { return first_tree; }

private:
	allocator<string> alloc;                                            //分配元素
																		//被添加元素的函数使用
	void chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}
	// 工具函数，被拷贝构造函数、赋值运算符和析构函数所使用
	pair<string*, string*> alloc_n_copy(const string*, const string*);
	void free();                                                            //销毁元素并释放内存
	void reallocate();                                                      //获得更多内存并拷贝已有元素
	string *elements;                                                       //指向数组首元素的指针
	string *first_tree;                                                     //指向数组第一个空闲元素的指针
	string *cap;                                                            //指向数组尾后指针的位置
};