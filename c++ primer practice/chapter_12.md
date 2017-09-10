# C++ primer 练习 - 12章：动态内存

## 12.1

b1,b2各有4个元素，因为StrBlob保存的是指针

## 12.2
<pre>
using namespace std;
class StrBlob
{
public:
    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() const {return data->size();}
    bool empty() const { return data->empty(); }
    //添加和删除元素
    void push_back(const string &t) {data->push_back(t);}
    void pop_back();
    //元素访问
    string& front() const;
    string& back() const;
private:
    shared_ptr<vector<string>> data;
    //如果data[i]不合法，抛出一个异常
    void check(size_type i, const string &msg) const;
};

void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size())
        throw out_of_range(msg);
}
void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    return data->pop_back();
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
</pre>

## 12.3
没有必要，因为`const`修饰成员变量函数是为了防止该函数对成员变量的修改。

## 12.4
因为`size_type`是一个无符号类型，即使为负数也会转成正数。

## 12.5
explicit的作用就是抑制构造函数的隐式转换

- 优点：不会自动的进行类型转换，必须清楚的知道类类型
- 缺点：必须用构造函数显示创建一个对象，不够方便简单

## 12.6
<pre>
using namespace std;
vector<int>* MakeVec()
{
    vector<int> *a = new vector<int>;
    return a;
}
vector<int>* GetVec(vector<int>* vec)
{
    int i;
    while(cin >> i)
    {
        vec->push_back(i);
    }
    return vec;
}
void PrintVec(vector<int>* vec)
{
    for(auto i = vec->cbegin(); i!=vec->cend(); i++)
        cout<< *i <<endl;
}

int main()
{
    vector<int> *vec = MakeVec();
    vec = GetVec(vec);
    PrintVec(vec);
    delete vec;
    vec = nullptr;
    system("pause");
    return 0;
}
</pre>

## 12.7
<pre>
using namespace std;
shared_ptr<vector<int>> MakeVec()
{
    shared_ptr<vector<int>> p(new vector<int>);             //分配空间
    return p;
}
//与第6题相似，此处可以不反回，因为函数内容为指针操作
shared_ptr<vector<int>> GetVec(shared_ptr<vector<int>> p)    
{
    int i;
    while(cin >> i)
    {
        p->push_back(i);
    }
    return p;
}
void PrintVec(shared_ptr<vector<int>> p)        
{
    for(auto i = p->cbegin(); i!=p->cend(); i++)
        cout<< *i <<endl;
}

int main()
{
    shared_ptr<vector<int>> p = MakeVec();
    p = GetVec(p);
    PrintVec(p);
    p = nullptr;
    system("pause");
    return 0;
}
</pre>

## 12.8
有问题，p是一个指针返回的是bool类型，这会导致p分配的空间无法得到释放。

## 12.9
<pre>
int *q = new int(42), *r = new int (100);
r = q;
</pre>
这里会导致r分配的空间无法得到释放。

## 12.10
正确

## 12.11
利用`P.get()`函数得到的内置指针来初始化一个临时的智能指针，一旦调用结束，该临时指针被销毁，临时指针的引用计数变为0而释放内存，使得p变为空悬指针。

## 12.12
<pre>
void process(shared_ptr<int> ptr)
{
	//使用ptr
}//ptr离开作用域，被销毁
</pre>
**`process()`只能接受智能指针**

- (a) 合法 sp为智能指针
- (b) 不合法 临时内置指针
- (c) 不合法 内置指针
- (d) 合法 临时智能指针

## 12.13
会释放sp所指向的空间，使得sp变为空悬指针。

## 12.14

	struct destination;							//表示我们正在连接什么
    struct connection;							//使用连接所需的信息
    connection connect(destination*);			//打开连接
    void disconnect(connection);				//关闭给定的连接
    
    void end_connection(connection *p)
    {
    	disconnect( *p );
    }
    void f(destination &d)
    {
    	connection c = connect(&d);
    	shared_ptr<connection> p(&c, end_connection);
    	//使用连接
    	//当f退出时（即使是异常退出），connection会被正确关闭
    }

**需要对声明进行定义即可使用**

## 12.15
`[](connection *p){ disconnect *p; } `

## 12.16
略

## 12.17
编辑器里除了(a)全部通过。怎么回事也不清楚

- (a) 不合法： ix不是new返回的指针
- (b) 不合法： 不支持拷贝
- (c) 合法： 不支持拷贝
- (d) 不合法： 不支持赋值
- (e) 合法
- (f) 不合法： `p2.get()`返回一个内置指针，`unique_ptr`不支持拷贝

## 12.18
release()函数的作用就是放弃对指针指向对象的控制权，但`shared_ptr`是多对一的关系，其他的智能指针仍然可以删除这个对象，所以这个函数的话对shared_ptr没意义。

## 12.19
    #include <iostream>
    #include <stdlib.h>
    #include <vector>
    #include <string>
    #include <memory>
    
    using namespace std;
    
    
    class StrBlob
    {
    	friend class StrBlobPtr;
    	StrBlobPtr begin();
    	StrBlobPtr end();
    public:
    	typedef vector<string>::size_type size_type;
    	StrBlob(): data(make_shared<vector<string>>()) {}
    	StrBlob(initializer_list<string> il);
    	StrBlob(string il);
    	size_type size() const { return data->size(); }
    	bool empty() const { return data->empty(); }
    	//添加和删除元素
    	void push_back(const string &t) { data->push_back(t); }
    	void pop_back();
    	//元素访问
    	string& front() const;
    	string& back() const;
    private:
    	shared_ptr<vector<string>> data;
    	//如果data[i]不合法，抛出一个异常
    	void check(size_type i, const string &msg) const;
    };
    
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
    
    
    class StrBlobPtr
    {
    public:
    	StrBlobPtr() : curr(0) {}
    	StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    	string& deref() const
    	{
    		auto p = check(curr, "deference past end");
    		return (*p)[curr];
    	}
    	StrBlobPtr& icur()			//前缀增加
    	{
    		auto p = check(curr, "deference past end");
    		++curr;
    		return *this;
    	}
    private:
    	//若检验成功，check返回一个指向vector的shared_ptr
    	shared_ptr<vector<string>> check(size_t i, const string& msg) const
    	{
    		auto ret = wptr.lock();
    		if (!ret)
    			throw runtime_error("unbound StrBolbPtr");
    		if (i >= ret->size())
    		{
    			throw out_of_range(msg);
    		}
    		return ret;
    	}
    	//保存一个weak_ptr，意味着底层vector可能被销毁
    	weak_ptr<vector<string>> wptr;
    	size_t curr;
    };
    
    StrBlobPtr StrBlob::begin()
    {
    	return StrBlobPtr(*this);
    }
    StrBlobPtr StrBlob::end()
    {
    	return StrBlobPtr(*this, data->size());
    }
    
    int main()
    {
    	StrBlob str = { "first", "second", "third"};
    	StrBlobPtr ptr = StrBlobPtr(str);
    	cout << ptr.deref() << endl;				//cout "first"
    	ptr.icur();
    	cout << ptr.deref() << endl;				//cout "second"
    
    
    	system("pause");
    	return 0;
    }

## 12.20
### 类结果与12.19相似，这里补充main函数
    int main()
    {
    	StrBlob str ;
    	ifstream infile("文件.txt");
    	string s;
    	while(getline(infile, s))
    	{
    		str.push_back(s);
    		cout << str.back() << endl;
    	}
    	for (StrBlobPtr pbeg = str.begin(), ebeg = str.end(); pbeg.deref() != ebeg.deref(); pbeg.icur())
    	{
    		cout << pbeg.deref() << endl;
    	}
    
    
    	system("pause");
    	return 0;
    }

# <font color = 'red'> 12.19和12.20的总结 </font>
--------------------
`StrBlob()`在没有形参时，需要创建一个`vector<string>`作为智能指针`data`的指向目标，否则对`data`进行操作就会报错。

也就是说，当智能指针指向的内容为一个容器时，通过指针对容器操作时必须先使指针指向一个容器，否则报错。

**看一段代码**

    int main()
    {
	    shared_ptr<vector<string>> ptr = make_shared<vector<string>>(vector<string>{});
	    ptr->push_back("aaaaa");
	    return 0;
    }

这种情况下没有问题，因为在创建指正指针ptr时候使它指向一个空的`vector<string>`容器。

    int main()
    {
	    shared_ptr<vector<string>> ptr;
	    ptr->push_back("aaaaa");
	    return 0;
    }
这种情况下就会报错，因为智能指针ptr未指向一个容器，自然不能进行容器操作了。

## 12.21
原先的版本好，可读性更强，

## 12.22
将StrBlobPtr的构造函数定义为const。

## 12.23
    using namespace std;
    int main(int argc, char**argv)
    {
    	char *s1 = "abc";
    	char *s2 = "efg";//字符串字面常量,字符串末尾有空格
    	string si = "a";
    	string sl = "b";//标准库string对象
    	char *p = new char[strlen(s1) + strlen(s2) + 1];//必须指明要分配对象的个数
    	strcpy(p, s1);//复制
    	strcat(p, s2);//接上
    	cout << p << endl;
    	strcpy(p, (si + sl).c_str());//必须转换为c类型字符串(c中无string类型)
    	cout << p << endl;
    	delete[] p;
    	system("pause");
    	return 0;
    }

## 12.24
长度超出会自动补上

    int main(int argc, char**argv)
    {
	    string a;
	    cin>>a;
	    char *p = new char[2];
	    strcpy(p,a.c_str());
	    cout<<p<<endl;
	    delete [] p;//一定要记得delete
	    system("pause");
	    return 0;
    }

## 12.25

    delete [] pa

## 12.26

	using namespace std;
	int main()
	{
		allocator<string> alloc;
		auto const p = alloc.allocate(10);
		auto q = p;
		string s;
		while (cin >> s && q != p + 10)
		{
			alloc.construct(q++, s);
		}
		while (p != q)
		{
			alloc.destroy(--q);
		}
		alloc.deallocate(p, 10);
	}

## 12.27~12.30(12.3自己写的程序)

### **main.cpp**

	#include "main.h"
	using namespace std;
	
	TextQuery::TextQuery(ifstream &infile)
	{
		string s;
		size_t lineNum = 0;
		while (getline(infile, s))
		{
			m_text.push_back(s);
			stringstream ss(s);				//字符流
			string word;
			while (ss >> word)
			{
				auto it = m_line.find(word);
				if (it != m_line.end())		//如果map中已有
					it->second.insert(lineNum);
				else
				{
					m_line.insert(make_pair(word, set<size_t>{lineNum}));
				}
			}
			lineNum++;
		}
	}
	
	QueryResult TextQuery::query(string &s)
	{
		auto it = this->m_line.find(s);
		shared_ptr<vector<string>> data = make_shared<vector<string>>(this->m_text);
		shared_ptr<set<size_t>> line;
		if (it != this->m_line.end())
			line = make_shared<set<size_t>>(it->second);
		else
			line = make_shared<set<size_t>>();			//指向空set
		return QueryResult(s, line, data);
	
	}
	
	
	void runQueries(ifstream &infile)
	{
		// infile是一个ifstream，指向我们需要处理的文件
		TextQuery tq(infile);
		// 与用户交互：提示用户输入要查询的单词，完成查询并打印结果
		while (true)
		{
			cout << "enter word to look for, or q to quit: ";
			string s;
			// 若遇到文件尾或用户输入了'q'时循环终止
			if (!(cin >> s) || s == "q")
				break;
			print(cout, tq.query(s)) << endl;
		}
	}
	
	ostream& print(ostream& os, const QueryResult &qr)
	{
		os << "element occyrs " << qr.m_row->size() << " times" << endl;
		for (set<size_t>::iterator it = qr.m_row->begin(); it != qr.m_row->end(); it++)
		{
			size_t l = *it;
			os << "( " << l+1 << " )\t";
			os << qr.m_data->at(l) << endl;
		}
		return os;
	}
	
	int main()
	{
		ifstream infile("文件.txt");
		runQueries(infile);
	}


### **main.h**

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
		vector<string> m_text;						//用于存放文本的每一行内容
		map<string, set<size_t>> m_line;			//set中存放每个单词的行号，用map进行关联
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


## 12.31

`vector`保存行号会使得在读取单词所属行时读入多个行数存入`vector`，但是这样可以实现每一行有多少个同样的单词，但是本例中不需要这个功能。

## 12.32

替换下类名就好了

## 12.33

	set<size_t>::iterator begin() const { return m_row->begin(); }
	set<size_t>::iterator end() const { return m_row->end(); }
	shared_ptr<vector<string>> get_file() const { return m_data; }

# <font color = 'red'> 最终结果在12.30题答案下面</font>

# 第12章：动态内存心得



###1. 主要掌握几个知识点，智能指针`shared_ptr`，`unique_ptr`，`weak_ptr`。

- `shared_ptr`具有计数器记录共享的智能指针个数，当计数器为0时释放空间。

	> 其使用`make_shared`产生在动态内存中分配一个对象并初始化，返回指向该对象的`shared_ptr`。
	
	> 不要混用智能指针和内置指针，不要用智能指针`.get()`来初始化另一个智能指针或为智能指针赋值。下面这个例子中会对计数器产生混乱。
	>
		using namespace std;
		int main()
		{
			shared_ptr<int> p(new int(42));
			int *q = p.get();
			shared_ptr<int> c(p);
			cout << p.use_count() << endl;
			system("pause");
			return 0;
		}

- `unique_ptr`是一对一的，一个指针对应一块区域，指针指向位置改变的时候也会释放空间。

- `weak_ptr`是弱的智能指针，指向由`sharped_ptr`管理的对象，该指针对计数器没有影响。

	> `weak_ptr`初始化需要使用一个`sharped_ptr`，比如`weak_ptr<int>wp(p)`，p就是一个`sharped_ptr`。
	> 
	> `weak_ptr`的对象不一定存在，所以需要`w.lock()`的使用，若存在返回对应的`sharped_ptr`，否则返回空的`sharped_ptr`。

###2. allocator类

一种自己管理内存的类，比new/delete更有效率，但是用起来还是挺麻烦的。

- `a.allocate(n)` : 分配内存,相当于new
- `a.construct(p, args)` : args被分配给p指向的内存位置，相当于我们在申请空间之后的赋值行为。
- `a.destroy(p)` ： 对p所指向的对象执行析构函数。
- `a.deallocate(p,n)` ： 释放从p开始的内存，p必须为创建时返回的指针，n必须为创建时分配的大小。相当于delete行为，但是更麻烦。


###3. 还有就是熟悉的new/delete了







