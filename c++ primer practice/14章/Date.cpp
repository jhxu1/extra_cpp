#include <iostream>

using namespace std;

class Date
{
	friend ostream& operator<<(ostream& os, const Date& d);
	friend istream& operator>>(istream &is, Date &d);
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

int main()
{
	Date d1(1990, 6, 4);
	Date d2(111, 7, 22);
	d2 += d1;
	cout << d2 << endl;
	system("pause");
	return 0;
}