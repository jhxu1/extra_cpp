#include <iostream>  
#include <string>  
#include <stdlib.h>
using namespace std;

class Sales_data
{
	// 声明友元函数  
	friend Sales_data operator+(const Sales_data&, const Sales_data&);
	friend ostream &operator<<(ostream&, const Sales_data&);
	friend istream &operator>>(istream&, Sales_data&);

public:

	// 构造函数  
	Sales_data():bookNo(""), units_sold(0), revenue(0.0){ }

	// 读取到当前对象中  
	Sales_data(istream &is) { cout<<(is, *this); }

	// 返回isbn编号  
	string isbn() const { return bookNo; }

	//拷贝赋值运算符
	Sales_data& operator=(const Sales_data &rhs)
	{
		this->bookNo = rhs.bookNo;
		this->revenue = rhs.revenue;
		this->units_sold = rhs.units_sold;
		return *this;
	}
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

	// 均价  
	double avg_price() const
	{
		if (units_sold)
		{
			return revenue / units_sold;
		}
		return 0;
	}

private:
	string bookNo; // isbn号码  
	unsigned units_sold; // 卖了多少册  
	double revenue; // 总收益  
};


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
	item.revenue = price * item.units_sold;
	return is;
}


// 友元函数：传入一个ostream对象cout， 一个Sales_data对象， 返回一个ostream对象的引用cout, 便于链式输出  
ostream& operator<<(ostream &os, const Sales_data &item)
{
	os << "图书编号：	"<<item.isbn() << "	销售数量：	" << item.units_sold << "	总收益：	 "
		<< item.revenue << "	均价：	" << item.avg_price();
	return os;
}

Sales_data operator+(const Sales_data &a, const Sales_data &b)
{
	Sales_data temp = a;
	temp += b;
	return temp;
}

int main()
{
	Sales_data total;
	if (cin>>(cin, total))  // 读取对象数据， 存入total中， 并返回cin   
	{
		Sales_data trans;
		if(cin >> (cin, trans))
		{
			if (total.isbn() == trans.isbn()) // 判断当前isbn和前一个isbn是否相等  
			{
				total = total + trans;  // 加到当前对象total中  
			}
			else
			{
				cout<<(cout, total) << endl; // 输出当前对象  
				total = trans;  // 更新当前对象  
			}
		}

		cout<<(cout, total) << endl; // 输出当前对象(最后的对象)  
	}
	else
	{
		cerr << "No data?!" << endl; // 最开始就读取失败  
	}
	system("pause");
	return 0;
}