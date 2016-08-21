#include"BigData.h"
#include<assert.h>
//若传入整数则直接调用下面构造函数
BigData::BigData (INT64 value)
	             :_value(value)
{}
//若传入字符串则调用下面构造函数
BigData::BigData (char* pdata)
{
//"122344455677789900"
//"+12345678901111"
//"-1234567890888"
//"000000123334565475870"
//"0xa1234588980099"
	assert(NULL!=pdata);
   char csymbol=pdata[0];//检查第一个字符是否有效

   //（1）输入字符串的首位为0-9之间的数字
   //"122344455677789900"
   if(csymbol>='0' && csymbol<='9')
   {
   csymbol='+';
   }

   //(2)输入字符串的首位是一个‘+’或‘-’则指针向前走一位
   //"+12345678901111"
   //"-1234567890888"
   else if('+'==csymbol||'-'==csymbol)
   {
      pdata++;
   }

   //(3)其他
   else
   {
	//给检查到的无效值赋上一个固定值
    _value=UN_INT;
   }

   //"000000123334565475870"
   while('0'==*pdata)
   {
   ++pdata;
   }
   _strdata.resize(strlen(pdata)+1);//重新分配空间并赋值
   _strdata[0]=csymbol;

   //存储转化后的数字
   int count=1;
   _value=0;
   while(*pdata>='0' && *pdata<='9')
   {
   _strdata[count++]=*pdata;
   _value=_value*10+ *pdata-'0';
   pdata++;
   }
   _strdata.resize (count);
   //若输入为负数，则将存储的数字转化为负数
   if(csymbol=='-')
   {
   _value=0-_value;
   }
   
}

BigData BigData::operator+(const BigData& bigdata)
{

	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow ())
	{
		//相加的两个数异号8,-2,10
		 if(_strdata[0]!=bigdata._strdata [0])
		 {
		  return BigData(_value+bigdata._value );
		 }
		 //相加的两个数同号2,8,10;-2，-8，-10；
		 else
		 {
	      if((('+')==_strdata[0] && MAX_INT64-_value>=bigdata._value) 
			 ||(('-')==_strdata[0] && MIN_INT64-_value<=bigdata._value ))
		  {
		    return BigData(_value+bigdata._value );
		  }
		 }
		 
	}
	    //溢出
		 if(_strdata[0]==bigdata._strdata [0])
		 {
		  return BigData((char*)Add(_strdata,bigdata._strdata).c_str ());
		 }
		  return BigData((char*)Sub(_strdata,bigdata._strdata).c_str ());

}
string BigData::Add(string left,string right)
{
	int Lsize=left.size();
	int Rsize=right.size();

	if(Lsize<Rsize)//保证最左边为最长字符串
	{
	swap(left,right);
	swap(Lsize,Rsize);
	}
	string strRet;//保存计算结果
	strRet.resize(Lsize+1);//
	strRet[0]=left[0];//确定相加结果的符号位
	char cstep=0;
	for(int iIdx=1;iIdx<Lsize;iIdx++)
	{
		char cret=left[Lsize-iIdx]-'0';

		if(iIdx<Rsize)
		{
		cret+=(right[Rsize-iIdx]+cstep-'0');
		}
		//将相加结果转化为字符
		strRet[Lsize-iIdx+1]=(cret%10+'0');
		//产生高位进位 
		cstep=cret/10;
	}
	//将最高位的进位结果同样转化为字符保存
	strRet[1]=(cstep+'0');
	return strRet;
}

BigData BigData::operator-(const BigData& bigdata)
{
	//两个数据都没有溢出并且同号，则直接相减
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
	  if(_strdata[0]==bigdata._strdata[0])
	  {
	   return BigData(_value-bigdata._value);
	  }
	  //3-（-6）=9
	  //3 ,-6, 10
	  //-3-6=-9
	  //-3, 6, -10
	  //两个数据都没有溢出，异号，相减的结果也没溢出
	  else
	  {
		  if(('+'==_strdata[0]&&MAX_INT64+bigdata._value >=_value)
			  ||('-'==_strdata[0]&&MIN_INT64 +bigdata._value<=_value))
		  {
			return BigData (_value-bigdata._value);
		  }
	  }
	}

//1至少有一个操作数溢出  11-3=8  8-（-3）=11
//2相减结果溢出 
	if(_strdata[0]==bigdata._strdata[0])
	{
		return BigData((char*)Sub(_strdata,bigdata._strdata).c_str ());
	}
	    return BigData((char*)Add(_strdata,bigdata._strdata).c_str ());
}
string BigData::Sub(string left,string right)
{
  int Lsize=left.size();
  int Rsize=right.size();
  char csymbol=left[0];

  //置左边为最大数
  //左 123       左 1111
  //右 1111     右 123
  //结果-         +
  //-1111左     左  123
  //123  右     右 -1111
  //结果-        +
  if(Lsize<Rsize || (Lsize==Rsize&& left<right))//string 类内已重载比较符号
  {//1确定正负号 
    swap(left,right);
	swap(Lsize,Rsize);

	if(left[0]=='+')
	{
		csymbol='-';
	}
	else
		csymbol='-';
  }

  string strRet;
  strRet.resize(Lsize);
  strRet[0]=csymbol;

  //逐位相减
  //1取出最左边的低位
  //2在右边低位没有超出的情况下取出右边低位
  //3直接相减
  //4保存结果
  for( int iIdex=1;iIdex<Lsize;iIdex++)
  {
	  char cRet=left[Lsize-iIdex]-'0';
	  if(iIdex<Rsize)
	  {
       cRet-=(right[Rsize-iIdex]-'0');
	  }
	  //左边低位<右边，则向前借位
	  if(cRet<0)
	  {
       left[Lsize-iIdex-1]-=1;
	   cRet+=10;
	  }
	  strRet[Lsize-iIdex]=(cRet+'0');
  }
  return strRet;
}

BigData BigData::operator*(const BigData& bigdata)
{
	if(0==_value ||0==bigdata._value)
		return BigData(INT64(0));
	//两个操作数无溢出
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
		
		 if(_strdata[0]==bigdata._strdata [0])
		 {
			 //同号并且结果没有溢出
			 if(('+'==_strdata[0] && MAX_INT64 /_value>=bigdata._value)
				 ||'-'==_strdata[0] && MAX_INT64 /_value<=bigdata._value)
			 {
			   return BigData(_value*bigdata._value); 
			 }

		 }
		 else
		 {
	       //异号并且结果没有溢出
			if(('-'==_strdata[0]&&MIN_INT64 /_value>=bigdata._value)
				||('+'==_strdata[0]&&MIN_INT64 /_value<=bigdata._value))
			{
			  return BigData(_value*bigdata._value);
			}
		 }
	}

	//产生溢出
	//1操作数溢出
	//2结果溢出
	return BigData((char*)Mul(_strdata,bigdata._strdata).c_str ());
}
string BigData::Mul(string left,string right)
{
	int Lsize=left.size();
	int Rsize=right.size();
	if(Lsize>Rsize)
	{
	  swap(left,right);
	  swap(Lsize,Rsize);

	}
	char csymbol='+';
	if(left[0]!=right[0])
	{
	csymbol='-';
	}
	string strRet;
	//c.assign (n,elem)将n个elem的拷贝赋值给c
	strRet.assign (Lsize+Rsize-1,'0');//出错主要忘记初始化的时候为字符0而不是数字0
	strRet[0]=csymbol;
	int DataLen=strRet.size();
	int offset=0;
	char step=0;
	for(int idex=1;idex<Lsize;idex++)
	{
		 char cleft=left[Lsize-idex]-'0';
		 char cstep=0;
		 if(cleft==0)
		 {
		 offset++;
		 continue;
		 }

		 for(int iRdex=1;iRdex<Rsize;iRdex++)
		 {
		 char cret=cleft*(right[Rsize-iRdex]-'0');
		 cret+=step;//
		 cret+=(strRet[DataLen-offset-iRdex]-'0');
		 strRet[DataLen-offset-iRdex]=cret%10+'0';
		 step=cret/10;
		 }
//次高位向最高位产生的进位
	   strRet[DataLen - offset - Rsize] += cstep;
		offset++;
	}
	return strRet;
}

 BigData BigData::operator/(const BigData& bigdata)
{
	//除数不能为0
	if(0==bigdata._value)
	{
	  assert(false);
	}
	//没有溢出
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
	  return BigData(_value/bigdata._value);
	}
	//溢出
	return BigData((char*)Div(_strdata,bigdata._strdata).c_str());

}
 string BigData:: Div(string left,string right)
 {
    char csymbol='+';
	if(left[0]!=right[0])
	{
	 csymbol='-';
	}
	int Lsize=left.size();
	int Rsize=right.size();
	//1 left<right
	if((Lsize<Rsize)||(Lsize==Rsize && strcmp(left.c_str()+1,right.c_str()+1)<0))
	{
	 return "0";
	}

	//2 绝对值left=right
	if(strcmp(left.c_str()+1,right.c_str()+1)==0)
	{
	//同号
	   if(left[0]==right[0])
	   {
		return "1";
	   }
		//异号
	   return "-1";
	}
	//3 如果right=-,+1
	    //+33333 +1
	if("+1"==right||"-1"==right)
	{
	  left[0]=csymbol;
	  return left;
	}

	string strRet;
	//string &append(int n,char c)在当前字符串结尾添加n个字符c
	strRet.append (1,csymbol);
	char* pLeft=(char*)(left.c_str()+1);
	char* pRight=(char*)(right.c_str()+1);
	int DataLen=1;
	for(int idex=0;idex<Lsize;)
	{
		if('0'==*pLeft)
		{
		  strRet.append (1,'0');
		  pLeft++;
		  idex++;
		  continue;
		}

		if(!IsLeftStrBig(pLeft,DataLen,pRight,Rsize-1))
		{
			strRet.append (1,0);
			DataLen++;
			if(idex+DataLen>Lsize)
			{
			 break;
			}
			continue;
		}

		else
		{
		//循环相减
			strRet.append(1,SubLoop(pLeft, DataLen, pRight, Rsize - 1));
		
		}
	  
	}
 }

 bool BigData::IsLeftStrBig(char *pLeft, size_t LSize, char *pRight, size_t RSize)
 {
   assert(pLeft!=NULL&& pRight!=NULL);
   if(LSize>RSize|| (LSize==RSize &&strncmp(pLeft,pRight,LSize)>=0))
   {
     return true;
   }
    return false;
 }

 char BigData::SubLoop(char *pLeft, size_t LSize, char *pRight, size_t RSize)
 {
    assert(pLeft!=NULL &&pRight!=NULL);
	char cRet='0';
	while(true)
	{
		 if(!IsLeftStrBig(pLeft,LSize,pRight,RSize))
		 {
			break;
		 }
		 int LDataLen=LSize-1;
		 int RDataLen=RSize-1;

		 while(LDataLen>=0 && RDataLen>=0)
		 {
		 
		 }

	}

 }
bool BigData::IsINT64Ooverflow()const
{
	string temp;
	//判断正负号
	if('+'==_strdata[0])
	{
	 temp=("+9223372036854775807");
	}
	
	if('-'==_strdata[0])
	{
	 temp=("-9223372036854775808");
	}
	if(_strdata.size()>temp.size())
	{
	 return true;
	}
	else if((_strdata.size()==temp.size())&&_strdata>temp)
	{
	  return true;
	}
	return false;
}

ostream& operator<<(ostream& os,const BigData& bigdata)
{
   //如果没有溢出
	if(!bigdata.IsINT64Ooverflow())
	{
		os<<bigdata._value;
	}
	//如果溢出
	else
	{
		char* pdata=(char*)bigdata._strdata .c_str();//字符串的首地址
		if('+'==bigdata._strdata[0])
		  {
		   pdata++; 
		  }
		os<<pdata;
	}
	return os;
}
