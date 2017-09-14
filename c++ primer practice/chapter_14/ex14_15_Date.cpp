#include <iostream>

using namespace std;

class Date
{
	friend ostream& operator<<(ostream& os, const Date& d);
	friend istream& operator>>(istream &is, Date &d);
	friend bool operator==(const Date& d1, const Date &d2);
	friend bool operator>(const Date& d1, const Date &d2);
	friend bool operator<(const Date& d1, const Date &d2);
public:
	Date() { }
	Date(int y, int m, int d) { year = y; month = m; day = d; }
	Date &operator+=(const Date &d)
	{
		day += d.day;
		if (day > 30)
		{
			day -= 30;
			month++;
		}
		month += d.month;
		if (month > 12)
		{
			month -= 12;
			year++;
		}
		year += d.year;
		return *this;
	}

private:
	int year, month, day;
};

ostream& operator<<(ostream& os, const Date& d)
{
	os << "year:" << d.year << "\t" << "month:" << d.month << "\t" << "day:" << d.day << endl;
	return os;
}

istream& operator>>(istream &is, Date &d)
{
	is >> d.year >> d.month >> d.day;
	if (!is)
		d = Date();
	return is;
}

bool operator==(const Date& d1, const Date &d2)
{
	return d1.day == d2.day && d1.month == d2.month && d1.year == d2.year;
}
bool operator!=(const Date& d1, const Date &d2)
{
	return !(d1 == d2);
}
bool operator>(const Date& d1, const Date &d2)
{
	return d1.year * 12 + d1.month * 30 + d1.day > d2.year * 12 + d2.month * 30 + d2.day;
}
bool operator<(const Date& d1, const Date &d2)
{
	return d1.year * 12 + d1.month * 30 + d1.day < d2.year * 12 + d2.month * 30 + d2.day;
}

int main()
{
	Date d1(1990, 6, 4);
	Date d2(1990, 6, 4);
	cout << (d2==d1) << endl;
	system("pause");
	return 0;
}