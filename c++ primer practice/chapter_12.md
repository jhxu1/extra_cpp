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
    data->pop_back();
}
string& StrBlob::front() const
{
    check(0, "pop_back on empty StrBlob");
    data->front();
}
string& StrBlob::back() const
{
    check(0, "pop_back on empty StrBlob");
    data->back();
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
