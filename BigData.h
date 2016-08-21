#pragma once
#include<iostream>
#include<string>
using namespace std;

typedef long long INT64;

#define UN_INT 0xCCCCCCCCCCCCCCC
#define MAX_INT64 0x7FFFFFFFFFFFFFFF
#define MIN_INT64 0x8000000000000000

class BigData
{
private:
	
    INT64 _value;
	string _strdata;

public:
	BigData(INT64=UN_INT);//若给出类似int a;类型的数据没有初始化，为了防止初始化时给用出随机值，所以UN_INT构造函数初始化
	BigData(char* pdata);

	BigData operator+(const BigData& bigdata);
	BigData operator-(const BigData& bigdata);
	BigData operator*(const BigData& bigdata);
	BigData operator/(const BigData& bigdata);
	friend ostream& operator<<(ostream& os,const BigData& bigdata);
private:

	string Add(string left,string right);
	string Sub(string left,string right);
	string Mul(string left,string right);
	string Div(string left,string right);

	bool IsINT64Ooverflow()const;
	bool IsLeftStrBig(char *pLeft, size_t LSize, char *pRight, size_t RSize);
	char SubLoop(char *pLeft, size_t LSize, char *pRight, size_t RSize);

    
};