#include <iostream>  
#include <string>  
using namespace std;

class Sales_data
{
	// 声明友元函数  
	friend ostream &print(ostream&, const Sales_data&);
	friend istream &read(istream&, Sales_data&);

public:

	// 构造函数  
	Sales_data()
		:bookNo(""), units_sold(0), revenue(0.0)
	{

	}

	// 读取到当前对象中  
	Sales_data(istream &is)
	{
		read(is, *this);
	}

	// 返回isbn编号  
	string isbn() const
	{
		return bookNo;
	}

	// 返回当前对象的引用  
	Sales_data& combine(const Sales_data& rhs)
	{
		units_sold += rhs.units_sold;
		revenue += rhs.revenue;
		return *this;
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
istream& read(istream &is, Sales_data &item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;

	return is;
}


// 友元函数：传入一个ostream对象cout， 一个Sales_data对象， 返回一个ostream对象的引用cout, 便于链式输出  
ostream& print(ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " "
		<< item.revenue << " " << item.avg_price();

	return os;
}


int main()
{
	Sales_data total;
	if (read(cin, total))  // 读取对象数据， 存入total中， 并返回cin   
	{
		Sales_data trans;
		while (read(cin, trans))  // 读取对象数据， 存入trans中， 并返回cin   
		{
			if (total.isbn() == trans.isbn()) // 判断当前isbn和前一个isbn是否相等  
			{
				total.combine(trans);  // 加到当前对象total中  
			}
			else
			{
				print(cout, total) << endl; // 输出当前对象  
				total = trans;  // 更新当前对象  
			}
		}

		print(cout, total) << endl; // 输出当前对象(最后的对象)  
	}
	else
	{
		cerr << "No data?!" << endl; // 最开始就读取失败  
	}

	return 0;
}