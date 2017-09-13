# C++ primer 练习 - 14章：重载运算和类型转换
---------------------------------

# 14.1 基本概念

## 14.1
当对类重载运算符时，对类的对象使用该运算符会调用重载运算符，对于非该类对象不使用重载运算符。

复合赋值运算符作为成员函数，具有对称性的运算符应该为非成员函数。

## 14.2

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

## 14.3
(a)应用了C++内置版本的==，比较两个指针。

(b) 应用了string版本的==。

(c)应用了vector版本的==。

(d)应用了string版本的==。

## 14.4
(a) %通常定义为非成员。

(b) %=通常定义为类成员，因为它会改变对象的状态。

(c) ++通常定义为类成员，因为它会改变对象的状态。

(d) ->必须定义为类成员，否则编译报错

(e) <<通常定义为非成员

(f) && 通常定义为非成员。

(g) ==通常定义为非成员。

(h) ()必须定义为类成员，否则编译会报错

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
# 14.2 输入和输出运算符

# 14.3 算数和关系运算符

# 14.4 赋值运算符

# 14.5 下标运算符

# 14.6 递增和递减运算符

# 14.7 成员访问运算符

# 14.8 重载调用运算符

# 14.9 重载、类型转换与运算符

# 小结