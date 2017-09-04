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

<font color = 'red'>****有错误****, 包括19.2</font>

using namesp    ace std;
    
    class StrBlob
    {
    	friend class StrBlobPtr;
    public:
    	typedef vector<string>::size_type size_type;
    	StrBlob();
    	StrBlob(initializer_list<string> il);
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
    
    void StrBlob::check(size_type i, const string &msg) const
    {
    	if (i >= data->size())
    		throw out_of_range(msg);
    }
    StrBlob::StrBlob(initializer_list<string> il)
    {
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
    	shared_ptr<vector<string>> check(size_t, const string&) const {};
    	//保存一个weak_ptr，意味着底层vector可能被销毁
    	weak_ptr<vector<string>> wptr;
    	size_t curr;
    	shared_ptr<vector<string>> check(size_t i, const string & s);
    };
    shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &s)
    {
    	auto ret = wptr.lock();
    	if (!ret)
    		throw runtime_error("unbound StrBolbPtr");
    	if (i >= ret->size())
    	{
    		throw out_of_range("out of range");
    	}
    	return ret;
    }