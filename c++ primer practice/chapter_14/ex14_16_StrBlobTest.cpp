#include "ex14_16_StrBlob.h"

using namespace std;
int main()
{
	StrBlob str1 = { "first", "second", "third" };
	StrBlob str2 = { "first", "second", "third" };
	cout << (str1 == str2) << endl;				//true
	StrBlobPtr ptr1 = StrBlobPtr(str1);
	StrBlobPtr ptr2 = StrBlobPtr(str1);
	ptr2.icur();
	cout << (ptr1 == ptr2) << endl;				//false


	system("pause");
	return 0;
}