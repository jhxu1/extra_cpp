# C++ primer 练习 - 14章：重载运算和类型转换
---------------------------------

# 14.1 基本概念

## Exercise 14.1
>在什么情况下重载的运算符与内置运算符有区别？在什么情况下重载的运算符又与内置运算符一样？

当对类重载运算符时，对类的对象使用该运算符会调用重载运算符，对于非该类对象不使用重载运算符。

复合赋值运算符作为成员函数，具有对称性的运算符应该为非成员函数。

## Exercise 14.2
> 为`Sales_data`编写重载的输入、输出、加法和符合复制运算符。

输入运算符

	istream& operator>>(istream &is, Sales_data &item)
	{
		double price = 0;
		cout << "输入图书编号:	";
		is >> item.bookNo;
		cout << "输入销售数量:	";
		is >> item.units_sold;
		cout << "输入图书单价:	";
		is >> price;
		item.revenue = price * item.units_sold;
		return is;
	}

输出运算符

	// 友元函数：传入一个ostream对象cout， 一个Sales_data对象， 返回一个ostream对象的引用cout, 便于链式输出  
	ostream& operator<<(ostream &os, const Sales_data &item)
	{
		os << "图书编号：	"<<item.isbn() << "	销售数量：	" << item.units_sold << "	总收益：	 "
			<< item.revenue << "	均价：	" << item.avg_price();
		return os;
	}

加法运算符
	
	Sales_data operator+(const Sales_data &a, const Sales_data &b)
	{
		Sales_data temp = a;
		temp += b;
		return temp;
	}

复合赋值运算符

	// 返回当前对象的引用  
	Sales_data& operator+=(const Sales_data& rhs)
	{
		if (rhs.bookNo == this->bookNo)
		{
			units_sold += rhs.units_sold;
			revenue += rhs.revenue;
			return *this;
		}
	}

## Exercise 14.3
> 在`String`和`vector`都定义了重载的`==`以比较各自的对象，假设`svec1`和`svec2`是存放`string`的vector，确定在下面的表达式中分别使用了哪个版本的`==`。
> 
> - (a) "cobble" == "stone" 
> - (b) "svec[0]" == svec2[0]
> - (c) svec1 == svec2
> - (d) "svec1[0] == "stone"


(a)应用了C++内置版本的==，比较两个指针。

(b)应用了string版本的==。

(c)应用了vector版本的==。

(d)应用了string版本的==。

## Exercise 14.4
> 如何确定下列运算符是否应该是类的成员？
> 
>  - (a) %
>  - (b) %=
>  - (c) ++
>  - (d) ->
>  - (e) <<
>  - (f) &&
>  - (g) ==
>  - (h) ()

(a) %通常定义为非成员。

(b) %=通常定义为类成员，因为它会改变对象的状态。

(c) ++通常定义为类成员，因为它会改变对象的状态。

(d) ->必须定义为类成员，否则编译报错

(e) <<通常定义为非成员

(f) && 通常定义为非成员。

(g) ==通常定义为非成员。

(h) ()必须定义为类成员，否则编译会报错


## Exercise 14.5 
> 在7.5.1节的练习7.40（第261页）中，编写了下列类中某一个的框架，请问在这个类中应该定义重载的运算符吗？如果是，请写出来。

选择了`Data`

	#include <iostream>
	using std::ostream;
	using std::endl;

	class Date
	{
	public:
		Date() { }
		Date(int y, int m, int d) {year = y; month = m; day = d;}
		friend ostream& operator<<(ostream &os, const Date &dt);
	
	private:
		int year, month, day;
	};
	
	ostream& operator<<(ostream& os, const Date& d)
	{
		const char sep = '\t';
		os << "year:" << d.year << sep << "month:" << d.month << sep << "day:" << d.day << endl;
		return os;
	}

## Exercise 14.6
> 为你的Sales_data 类定义输出运算符。

	// 友元函数：传入一个ostream对象cout， 一个Sales_data对象， 返回一个ostream对象的引用cout, 便于链式输出  
	ostream& operator<<(ostream &os, const Sales_data &item)
	{
		os << "图书编号：	" << item.isbn() << "	销售数量：	" << item.units_sold << "	总收益：	 "
			<< item.revenue << "	均价：	" << item.avg_price();
		return os;
	}

## Exercise 14.7
> 你在13.5节的练习（470页）中曾经编写了一个`String`类，为它定义一个输出运算符。


	ostream &operator << (ostream &os, const String s)
	{
		for (auto i = s.begin; i != s.end; i++)
			os << *i;
		return os;
	}


## Exercise 14.8
> 你在7.5.1节的练习7.40（第261）页中曾经选择并编写了一个类，为它定义一个输出运算符。

see [Exercise 14.5](#exercise-145)

## Exercise 14.9
> 为你的`Sales_data`类定义输入运算符。

	// 友元函数：传入一个istream对象cin, 一个Sales_data对象， 返回一个istream对象的引用cin， 便于连输输入  
	istream& operator>>(istream &is, Sales_data &item)
	{
		double price = 0;
		cout << "输入图书编号:	";
		is >> item.bookNo;
		cout << "输入销售数量:	";
		is >> item.units_sold;
		cout << "输入图书单价:	";
		is >> price;
		if (is)
			item.revenue = price * item.units_sold;
		else
			item = Sales_data();
		return is;
	}

## Exercise 14.10
> 对于`Sales_data`的输入运算符来说如果给定了下面的输入将发生什么情况？
>
> - (a) 0-201-99999-9 10 24.95
> - (b) 10 24.95 0-210-99999-9

- (a) 正常生成一个`Sales_data`类，其图书编号为`0-201-99999-9`，销售数量为`10`，图书单价为`24.95`，并生成对应的总价格。
- (b) 由于输入的数据类型错误，会生成一个空的`Sales_data`对象。

## Exercise 14.11
> 下面的Sales_data输入运算符存在错误吗，若果有，请指出来。对于这个输入运算符如果仍然给定上个练习的输入将会发生什么情况？

```cpp
istream& operator>>(istream& in, Sales_data& s)
{
    double price;
    in >> s.bookNo >> s.units_sold >> price;
    s.revenue = s.units_sold * price;
    return in;
}
```

没有输入检查，(b)物品单价会出错。

## Exercise 14.12
> 你在7.5.1节的练习7.40（第261页）中曾经选择并编写了一个类，为它定义一个输入运算符并确保该运算符可以处理输入错误。

`Data`
see [class Date](Date.cpp)

# 14.2 输入和输出运算符

# 14.3 算数和关系运算符

# 14.4 赋值运算符

# 14.5 下标运算符

# 14.6 递增和递减运算符

# 14.7 成员访问运算符

# 14.8 重载调用运算符

# 14.9 重载、类型转换与运算符

# 小结