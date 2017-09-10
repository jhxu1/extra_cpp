# C++ primer 练习 - 13章：拷贝控制
---------------------------------
部分内容参考博客：http://blog.csdn.net/misayaaaaa/article/details/53786215#comments

# 13.1.1 节练习： 拷贝构造函数
## 13.1
拷贝构造函数：本身是一个构造函数，其参 数是一个自身类类型的引用，且任何额外参数皆有默认值。

## 13.2
这是一个类的拷贝构造函数声明，需要使用引用类型的参数进行初始化。若使用非引用类型参数初始化时，会调用拷贝构造函数拷贝实参，但为了拷贝实参又需要拷贝构造函数，会进行无限循环。

## 13.3
StrBlob拷贝，创建新的智能指针指向原StrBlob类的`vector<string>`，智能指针计数器+1。

StrBlobPtr是弱智能指针，计数器不增加。

## 13.4
- `foo_bar`函数的参数为非引用类型，需拷贝。
- 在函数体中`arg`拷贝到`local`对象。
- `local`拷贝到`*heap`为拷贝赋值运算符
- `local`、`*heap`拷贝到`pa[4]`中皆使用拷贝构造函数。
- 使用拷贝构造函数、函数的返回类型非引用，也需要进行拷贝，使用拷贝构造函数。

## 13.5
	class HasPtr
	{
	public:
		HasPtr(const string &s = string()): ps(new string(s)), i(0){}
		HasPtr(const HasPtr &ptr):ps(new string(*ptr.ps)), i(ptr.i) {}
	private:
		string *ps;
		int i;
	};
	int main()
	{
		HasPtr a = HasPtr("1111");
		HasPtr b (a);
		cout << "数据" << endl;
		cout << *a.ps << "\t" << *b.ps << endl;			//1111    1111
		cout << "地址" << endl;
		cout << a.ps << "\t" << b.ps << endl;			//011BEC78        011BF020
		system("pause");
		return 0;
	}
结果相同，地址不同。

# 13.1.2 节练习：拷贝赋值运算符

## 13.6
- 拷贝赋值运算符：即重载`=`运算符。
- 何时调用：使用`=`将对象的值复制给一个已经存在的实例。
- 合成拷贝赋值完成工作：将右侧运算对象的每个非static`成员赋值给左侧运算对象的对应成员。
- 什么时候生成合成拷贝赋值运算符： 如果一个类未定义自己的拷贝赋值运算符。

## 13.7
- `StrBlob`：创建一个新的智能指针指向同一个`vector<string>`，计数器+1。
- `StrBlobPtr`：同上，由于是弱智能指针，计数器不增加。

## 13.8
	class HasPtr
	{
		friend int main();
	public:
		HasPtr(const string &s = string()) : ps(new string(s)), i(0) {}
		HasPtr(const HasPtr &ptr) :ps(new string(*ptr.ps)), i(ptr.i) {}
		HasPtr& operator = (const HasPtr &ptr)
		{
			delete ps;				//删除原先所指的string
			ps = new string(*ptr.ps);
			i = ptr.i;
			return *this;
		}
	private:
		string *ps;
		int i;
	};
	int main()
	{
		HasPtr a = HasPtr("1111");
		HasPtr b(a);
		cout << "数据" << endl;
		cout << *a.ps << "\t" << *b.ps << endl;			//1111    1111
		cout << "地址" << endl;
		cout << a.ps << "\t" << b.ps << endl;			//011BF460        011BF268
		system("pause");
		return 0;
	}
结果相同，地址不同。

#13.1.3 析构函数
## 13.9
- 析构函数：释放对象使用的资源，并销毁对象的非static数据成员。
- 合成析构函数：合成析构函数按对象创建时的逆序撤销每个非static成员，因此，它按成员在类中声明次序的逆序撤销成员。对于类类型的成员，合成析构函数调用该成员的析构函数来撤销对象。
- 合成析构函数生成时间：当一个对象被销毁时。
- 当对象的引用或指针超出作用域时，不会运行析构函数，只有删除指向动态分配对象的指针或实际对象（而不是对象的引用）超出作用域时，才会运行析构函数。<font color='red'>（也就是说当一个对象的引用或者未释放空间的指针离开作用域时，析构函数不会执行）</font>。

## 13.10
- `StrBlob`：所有数据成员被销毁，智能指针的计数器-1。
- `StrBlobPtr`：同上，由于是弱智能指针，计数器不减少。

## 13.11
	~HasPtr()
	{
		delete ps;
	}

## 13.12
3次，`accum`，`item1`，`item2`

## 13.13
	class X
	{
	public:
		X(int a):val(a) { cout << "默认构造函数" << endl; }
		X(const X &x):val(x.val){ cout << "拷贝构造函数" << endl; }
		X& operator=(const X &x)
		{
			val = x.val;
			cout << "赋值构造函数" << endl;
			return *this;
		}
		~X()
		{
			cout << "析构函数" << endl;
		}
		int val;
	
	};
	int main()
	{
		X x1(10);		//默认构造函数
		X x2(x1);		//拷贝构造函数
		X x3 = x1;		//拷贝构造函数
		X x4(4);		//默认构造函数
		x4 = x1;		//拷贝赋值运算符
		system("pause");
		return 0;
	}

# 13.1.4 三/五法则

## 13.14
会输出一样的值，因为合成的拷贝构造函数。

## 13.15
会改变，三个不一样的数字。

## 13.16
与15题结果不同，但仍然是不一样的数字。

## 13.17
	class numbered
	{
	public:
		numbered()				//默认构造函数
		{
			static int s_val = 0;
			val = s_val++;
		}
		numbered(const numbered &n)		//拷贝构造函数
		{
			val = n.val+10;
		}
		numbered& operator=(const numbered &n)	//拷贝赋值运算符
		{
			val = n.val + 3;
			return *this;
		}
		int val;
	};
	void f(numbered s)
	{
		cout << s.val << endl;
	}
	void f1(const numbered &s)
	{
		cout << s.val << endl;
	}
	
	int main()
	{
		numbered a1, a2;
		f(a1); f(a2);		//10	11
		f1(a1); f1(a2);		//0		1
		numbered a3 = a1;
		f(a3);				//20
		f1(a3);				//10
		numbered a4;
		a4 = a1;
		f(a4);				//13
		f1(a4);				//3
		system("pause");
		return 0;
	}

# 13.1.6 阻止拷贝
## 13.18
	class Employee
	{
	public:
		Employee(string n)
		{
			static size_t s_num = 1;
			num = s_num ++;
			name = n;
		}
		size_t num;
		string name;
	};

## 13.19
不需要拷贝控制成员，创建一个员工类的时候你会去找一个名字相同的员工？ 而且编号的处理也会混乱。

## 13.20
所有成员（包括智能指针和容器）被拷贝或销毁。

## 13.21
因为使用的是智能指针，不需要自己定义析构函数。

`TextQuery`类是保存数据，且没用到指针所以不需要定义拷贝控制成员。

`QueryResult`类有指向`TextQuery`成员的智能指针，这个类的目的就是读取对应`TextQuery`类的文本，虽然这里使用拷贝构造没有意义，但是使用合成拷贝构造也是可以的。所以没有必要再定义拷贝控制成员。

# 13.2 拷贝控制和资源管理

## 13.22，13.23 
见13.8
增加一个析构函数

	~HasPtr()
	{
		delete ps;
	}

## 13.24
如果没有析构函数，指针不会释放而导致内存泄漏。若未定义拷贝构造函数，合成拷贝构造函数会直接拷贝指针，而非指针所指向的值。

## 13.25
拷贝构造函数和拷贝赋值运算符需要为智能指针`data`分配空间。

因为合成析构函数会销毁智能指针，而智能指针会释放空间。所以不需要析构函数。

## 13.26
	StrBlob::StrBlob(const StrBlob &s)
	{
		data = make_shared<vector<string>>(*s.data);
	}
	StrBlob& StrBlob::operator=(const StrBlob &s)
	{
		data = make_shared<vector<string>>(*s.data);
		return *this;
	}

## 13.27
	class HasPtr
	{
	public:
		HasPtr(const string &s = string()) :ps(new string(s)),use(new size_t(1)) {}
		HasPtr(const HasPtr &s) :ps(s.ps), use(s.use) { ++*use; }
		~HasPtr()
		{
			if (--*use == 0)
			{
				delete ps;
				delete use;
			}
		}
		HasPtr& operator=(const HasPtr &s)
		{
			if (--*use == 0)
			{
				delete use;
				delete ps;
			}
			use = s.use;
			ps = s.ps;
			++*use;
			return *this;
		}
		void OutUse()
		{
			cout << *this->use << endl;;
		}
	private:
		string *ps;
		size_t *use;
	};
	
	int main()
	{
		//构造
		HasPtr p1; 
		p1.OutUse();	//1
		HasPtr p2;
		p2.OutUse();	//1
		//拷贝构造
		HasPtr p3(p1);
		p1.OutUse();	//2
		p3.OutUse();	//2
		//拷贝赋值
		p2 = p1;
		p2.OutUse();	//3
		system("pause");
		return 0;
	}

## 13.28
类似于27题

#13.3 交换操作

## 13.29
因为`swap`函数中的`swap`是std版本中的，而第一个`swap`是`HasPtr`的。

## 13.30
类值版本的`HasPtr`编写`Swap`函数

	class Hasptr
	{
		friend void swap(Hasptr&, Hasptr&);
	public:
		//默认构造函数 
		Hasptr(const string &s, int a=0):ps(new string(s)),i(a) {}
	
		//拷贝构造函数，完成string 指针指向内容的拷贝和i值的拷贝  
		Hasptr(const Hasptr& p) :ps(new string(*p.ps)), i(p.i) {}
	
		//拷贝赋值运算符  
		Hasptr& operator= (const Hasptr& p)
		{
			auto new_ps = new string(*p.ps);
			delete ps;
			ps = new_ps;
			return *this;
		}
	
		//析构函数  
		~Hasptr() { delete ps; }
	
		//输出ps
		void OutPs()
		{
			cout << *ps << endl;
		}
	private:
		string *ps;
		int i;
	};
	
	inline void swap(Hasptr& a, Hasptr& b)
	{
		using std::swap;
		swap(a.ps, b.ps);
		std::swap(a.i, b.i);
		cout << "swap" << endl;
	}
	
	int main()
	{
		Hasptr p1("p1"); Hasptr p2("p2");
		p1.OutPs();			//p1
		p2.OutPs();			//p2
		swap(p1, p2);
		p1.OutPs();			//p2
		p2.OutPs();			//p1
		system("pause");
		return 0;
	}

## 13.31
在上题代码中添加

	//<运算符
	bool operator< (const Hasptr &p)
	{
		cout << "<" << endl;
		return ps->size() < p.ps->size();
	}

以及输出检验函数

	int main()
	{
		vector<Hasptr> vec;
		vec.push_back(Hasptr("11111"));
		vec.push_back(Hasptr("222"));
		vec.push_back(Hasptr("333"));
		vec.push_back(Hasptr("4444"));
		vec.push_back(Hasptr("55555"));
		sort(vec.begin(),vec.end());
		for(auto pt : vec)
		{
			cout << *pt.ps << endl;
		}
		system("pause");
		return 0;
	}

## 13.32
类指针版本的`HasPtr`直接使用自带的`swap()`即可，自己写一个效果也是一样的。

# 13.4 拷贝控制示例

## 13.33
- 不能使用`(Folder)`： 如果不引用的话会拷贝一个Folder对象，而我们在往`folders`set中添加指针时指向的对象就不是我们希望的对象了，而是拷贝的那个对象。
- 不能使用`(const Folder &)`: 因为在`save`中使用了`Folder`的成员函数`addMsg`，这会对`Folder`的set进行更改，所以不能用const修饰。

##13.34

### main.cpp
	#include "main.h"
	using namespace std;
	
	void Message::save(Folder &f)
	{
		folders.insert(&f);
		f.addMsg(this);
	}
	
	void Message::remove(Folder &f)
	{
		folders.erase(&f);
		f.remMsg(this);
	}
	
	//将本Message添加到指向m的Folder中
	void Message::add_to_Folders(const Message &m)
	{
		for (auto i : m.folders)
		{
			i->addMsg(this);
		}
	}
	
	//移除指向本Message的Folder
	void Message::remove_from_Folders()
	{
		for (auto i : folders)
		{
			i->remMsg(this);
		}
	}
	
	//拷贝控制函数
	Message::Message(const Message &m):contents(m.contents),folders(m.folders)
	{
		add_to_Folders(m);
	}
	
	Message::~Message()
	{
		remove_from_Folders();
	}
	
	Message& Message::operator=(const Message &m)
	{
		//通过先删除指针再插入它们来处理自赋值情况
		remove_from_Folders();
		contents = m.contents;
		folders = m.folders;
		add_to_Folders(m);
		return *this;
	}
	
	void Folder::addMsg(Message *m)
	{
		messages.insert(m);
	}
	
	void Folder::remMsg(Message *m)
	{
		messages.erase(m);
	}
	
	Folder::~Folder()
	{
		for (auto i : messages)
		{
			i->remove(*this);
		}
	}
	
	
	int main()
	{
		Folder f1("f1");
		Message m1("m1");
		m1.save(f1);
		system("pause");
		return 0;
	}

### main.h
	#include <memory>
	#include <string>
	#include <set>
	#include <stdlib.h>
	
	
	using namespace std;
	
	class Message
	{
		friend class Folder;
	public:
		//folders被隐式初始化为空集合
		Message(const string &str = ""):contents(str) {}
		//拷贝控制成员，用来管理指向本Message的指针
		Message(const Message&);
		Message& operator=(const Message&);
		~Message();
		//从给定Folder集合中添加/删除本Message
		void save(Folder &);
		void remove(Folder &);
	
	private:
		string contents;				//实际消息文本
		set<Folder*> folders;			//包含本Message的Folder
		//拷贝构造函数、拷贝赋值运算符和析构函数所用的工具函数
		//将本Message添加到指向参数的Folder中
		void add_to_Folders(const Message&);
		//从folders中的每个Folder中删除本Message
		void remove_from_Folders();
	};
	
	class Folder
	{
		friend class Message;
	public:
		Folder(const string str = ""):name(str) {}
		Folder(const Folder &) = delete;
		~Folder();
		Folder operator=(const Folder&) = delete;
		void addMsg(Message*);			//往messages中添加
		void remMsg(Message*);			//在messages中删除
	private:
		string name;
		set<Message*> messages;
	};

## 13.35
在`Folder`的set中将不会添加新的`Message`地址。

## 13.36
见 13.34

## 13.37
	void Message::addFolder(Folder *f)
	{
		folders.insert(f);
	}
	
	void Message::remFolder(Folder *f)
	{
		folders.erase(f);
	}

## 13.38
当涉及到动态分配内存时,拷贝并交换是一个完成该功能的精简的方式. ，但是在Message类中,并未涉及到动态分配内存,这种方法并不会产生任何益处，同时还会因为很多指针操作让程序变得更复杂难难以实现。

# 13.5 动态内存管理

## 13.39
### main.cpp
#include "main.h"

	using namespace std;
	void StrVec::push_back(const string &s)
	{
		chk_n_alloc();			//确保有空间容纳元素
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
		auto dest = newdata;			//指向新数组中下一个空闲位置
		auto elem = elements;			//指向旧数组中下一个元素
		for (size_t i = 0; i != size(); ++i)
			alloc.construct(dest++, move(*elem++));
		//释放旧内存空间
		free();
		elements = newdata;
		first_tree = dest;
		cap = elements + newcapacity;
	}
	
	int main()
	{
		StrVec s1;
		s1.push_back("aaa");
		system("pause");
		return 0;
	}

### main.h
	#include <allocators>
	#include <memory>
	#include <string>
	#include <stdlib.h>
	
	using namespace std;
	// 类vector类内存分配策略的简化实现
	class StrVec
	{
	public:
		StrVec() :elements(nullptr), first_tree(nullptr), cap(nullptr){}		//默认初始化
		StrVec(const StrVec&);													//拷贝构造函数
		~StrVec();																//析构函数
		StrVec &operator=(const StrVec&);										//拷贝赋值运算符
		void push_back(const string&);											//拷贝元素
		size_t size() const { return first_tree - elements; }	
		size_t capacity() const { return cap - elements; }
		string *begin() const { return elements; }
		string *end() const { return first_tree; }
	
	private:
		allocator<string> alloc;											//分配元素
		//被添加元素的函数使用
		void chk_n_alloc()
		{
			if (size() == capacity())
				reallocate();
		}
		// 工具函数，被拷贝构造函数、赋值运算符和析构函数所使用
		pair<string*, string*> alloc_n_copy(const string*, const string*);
		void free();															//销毁元素并释放内存
		void reallocate();														//获得更多内存并拷贝已有元素
		string *elements;														//指向数组首元素的指针
		string *first_tree;														//指向数组第一个空闲元素的指针
		string *cap;															//指向数组尾后指针的位置
	};

## 13.40
	StrVec::StrVec(initializer_list<string> lst)
	{
		auto newdata = alloc_n_copy(lst.begin(), lst.end());
		elements = newdata.first;
		first_tree = newdata.second;
	}

## 13.41
如果后置会在第一个位置没有数据，并在最后在未分配的内存中插入数据，会出错。

## 13.42
`StrVec`包含了`string`的基本功能，可以替换使用。

##13.43
	for_each(elements, first_tree, [this](string &s) {alloc.destroy(&s);});

##13.44
	#include <allocators>
	#include <memory>
	#include <string>
	#include <stdlib.h>
	#include <algorithm>
	#include <iostream>
	
	using namespace std;
	class String
	{
		friend int main();
	public:
		String() :begin(nullptr), end(nullptr) {}
		String(const char *s, size_t size = 0)
		{
			auto newdata = alloc_n_copy(s, s+size);
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

#13.6 对象移动

## 13.45

- 左值引用，也就是“常规引用”，不能绑定到要转换的表达式，字面常量，或返回右值的表达式。而右值引用恰好相反，可以绑定到这类表达式，但不能绑定到一个左值上。

- 右值引用就是必须绑定到右值的引用，通过&&获得。右值引用只能绑定到一个将要销毁的对象上，因此可以自由地移动其资源。

- 返回左值的表达式包括返回左值引用的函数及赋值，下标，解引用和前置递增/递减运算符，返回右值的包括返回非引用类型的函数及算术，关系，位和后置递增/递减运算符。可以看到左值的特点是有持久的状态，而右值则是短暂的。

## 13.46

`int f()`：非引用函数为右值，所以可以用右值引用
`vector<int> vi(100)`：为左值，用左值引用

(a) `int &&r1 = f();`
(b) `int &r2 = vi[0];`
(c) `int &r3 = r1;`
(d) `int &&r4 = vi[0]*f();`

## 13.47,13.48
！[image](https://raw.githubusercontent.com/AugusXJ/cpp/master/c%2B%2B%20primer%20practice/image/13.47.PNG)

当`push`一个时会拷贝一次，如果内存不够则会开辟一块新内存，再把`vector`中已有的进行拷贝，再继续`push`。

> 因此`push`一次就拷贝了一次。

> `push`两次就是1+1+1=3次，第一个1为`push`第一个数; 第二个1是在`push`第二个数时发现空间不够，将现有的一个数拷贝到更大的空间; 第三个1是`push`第二个数。可以从图中看到三个1是如何增加的。

> `push`3个string就是3+2+1=6，4个就是6+3+1=10，5个就是10+4+1=15 ... 以此类推
