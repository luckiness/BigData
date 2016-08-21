#include"BigData.h"

void TestBigDataAdd()
{   
	cout<<"---------TestBigDataAdd---------"<<endl;
	BigData b1("999999999");
	BigData b2("000000001");
	cout<<"两个没有溢出的同号操作数相加并且结果未溢出"<<endl;
	cout<<"b1+b2=:";
	cout<<b1+b2<<endl;

	cout<<"两个没有溢出的异号操作数相加并且结果未溢出"<<endl;
	BigData b3("999999999");
	BigData b4("-1");
	cout<<"b3+b4:";
	cout<<b3+b4<<endl;

	cout<<"两个没有溢出的同号操作数相加但结果溢出"<<endl;
	BigData b5("922337203685477580");
	BigData b6("9223372036854775806");
	cout<<"b5+b6:";
	cout<<b5+b6<<endl;

	cout<<"两个溢出的同号操作数相加结果溢出"<<endl;
	BigData b7("92233720368547758071");
	BigData b8("92233720368547758072");
	cout<<"b7+b8:";
	cout<<b7+b8<<endl;
}


void TestBigDataSub()
{
cout<<"---------TestBigDataSub---------"<<endl;
cout<<"两个无溢出的同号操作数"<<endl;
	BigData b1("999999999");
	BigData b2("000000001");
cout<<"b1-b2:"<<b1-b2<<endl;
cout<<"两个无溢出的异号操作数"<<endl;
    BigData b3("999999999");
	BigData b4("-000000001");
cout<<"b3-b4:"<<b3-b4<<endl;
cout<<"操作数或者操作结果溢出"<<endl;
    BigData b5("92233720368547758071");
	BigData b6("-000000001");
cout<<"b5-b6:"<<b5-b6<<endl;
}

void TestBigDataMul()
{
	cout<<"---------TestBigDataMul---------"<<endl;
	cout<<"两个同正号操作数操作结果没溢出"<<endl;
    BigData b1("999999999");
	BigData b2("000000001");
	cout<<"b1*b2:"<<b1*b2<<endl;
	cout<<"两个同负号操作数操作结果没溢出"<<endl;
	BigData b3("-999999999");
	BigData b4("-000000001");
	cout<<"b3*b4:"<<b3*b4<<endl;
	cout<<"两个异号操作数操作结果没溢出"<<endl;
	BigData b5("-999999999");
	BigData b6("000000001");
	cout<<"b5*b6:"<<b5*b6<<endl;
	cout<<"溢出"<<endl;
	BigData b7("922337203685477580719999");
	BigData b8("1");
	cout<<"b7*b8:"<<b7*b8<<endl;
	cout<<"溢出"<<endl;
	BigData b9("9223372036854775807199990");
	BigData b10("1");
	cout<<"b9*b10:"<<b9*b10<<endl;


}
int main()
{
//TestBigDataAdd();
	TestBigDataMul();
   system("pause");
   return 0;
}