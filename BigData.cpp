#include"BigData.h"
#include<assert.h>
//������������ֱ�ӵ������湹�캯��
BigData::BigData (INT64 value)
	             :_value(value)
{}
//�������ַ�����������湹�캯��
BigData::BigData (char* pdata)
{
//"122344455677789900"
//"+12345678901111"
//"-1234567890888"
//"000000123334565475870"
//"0xa1234588980099"
	assert(NULL!=pdata);
   char csymbol=pdata[0];//����һ���ַ��Ƿ���Ч

   //��1�������ַ�������λΪ0-9֮�������
   //"122344455677789900"
   if(csymbol>='0' && csymbol<='9')
   {
   csymbol='+';
   }

   //(2)�����ַ�������λ��һ����+����-����ָ����ǰ��һλ
   //"+12345678901111"
   //"-1234567890888"
   else if('+'==csymbol||'-'==csymbol)
   {
      pdata++;
   }

   //(3)����
   else
   {
	//����鵽����Чֵ����һ���̶�ֵ
    _value=UN_INT;
   }

   //"000000123334565475870"
   while('0'==*pdata)
   {
   ++pdata;
   }
   _strdata.resize(strlen(pdata)+1);//���·���ռ䲢��ֵ
   _strdata[0]=csymbol;

   //�洢ת���������
   int count=1;
   _value=0;
   while(*pdata>='0' && *pdata<='9')
   {
   _strdata[count++]=*pdata;
   _value=_value*10+ *pdata-'0';
   pdata++;
   }
   _strdata.resize (count);
   //������Ϊ�������򽫴洢������ת��Ϊ����
   if(csymbol=='-')
   {
   _value=0-_value;
   }
   
}

BigData BigData::operator+(const BigData& bigdata)
{

	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow ())
	{
		//��ӵ����������8,-2,10
		 if(_strdata[0]!=bigdata._strdata [0])
		 {
		  return BigData(_value+bigdata._value );
		 }
		 //��ӵ�������ͬ��2,8,10;-2��-8��-10��
		 else
		 {
	      if((('+')==_strdata[0] && MAX_INT64-_value>=bigdata._value) 
			 ||(('-')==_strdata[0] && MIN_INT64-_value<=bigdata._value ))
		  {
		    return BigData(_value+bigdata._value );
		  }
		 }
		 
	}
	    //���
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

	if(Lsize<Rsize)//��֤�����Ϊ��ַ���
	{
	swap(left,right);
	swap(Lsize,Rsize);
	}
	string strRet;//���������
	strRet.resize(Lsize+1);//
	strRet[0]=left[0];//ȷ����ӽ���ķ���λ
	char cstep=0;
	for(int iIdx=1;iIdx<Lsize;iIdx++)
	{
		char cret=left[Lsize-iIdx]-'0';

		if(iIdx<Rsize)
		{
		cret+=(right[Rsize-iIdx]+cstep-'0');
		}
		//����ӽ��ת��Ϊ�ַ�
		strRet[Lsize-iIdx+1]=(cret%10+'0');
		//������λ��λ 
		cstep=cret/10;
	}
	//�����λ�Ľ�λ���ͬ��ת��Ϊ�ַ�����
	strRet[1]=(cstep+'0');
	return strRet;
}

BigData BigData::operator-(const BigData& bigdata)
{
	//�������ݶ�û���������ͬ�ţ���ֱ�����
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
	  if(_strdata[0]==bigdata._strdata[0])
	  {
	   return BigData(_value-bigdata._value);
	  }
	  //3-��-6��=9
	  //3 ,-6, 10
	  //-3-6=-9
	  //-3, 6, -10
	  //�������ݶ�û���������ţ�����Ľ��Ҳû���
	  else
	  {
		  if(('+'==_strdata[0]&&MAX_INT64+bigdata._value >=_value)
			  ||('-'==_strdata[0]&&MIN_INT64 +bigdata._value<=_value))
		  {
			return BigData (_value-bigdata._value);
		  }
	  }
	}

//1������һ�����������  11-3=8  8-��-3��=11
//2��������� 
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

  //�����Ϊ�����
  //�� 123       �� 1111
  //�� 1111     �� 123
  //���-         +
  //-1111��     ��  123
  //123  ��     �� -1111
  //���-        +
  if(Lsize<Rsize || (Lsize==Rsize&& left<right))//string ���������رȽϷ���
  {//1ȷ�������� 
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

  //��λ���
  //1ȡ������ߵĵ�λ
  //2���ұߵ�λû�г����������ȡ���ұߵ�λ
  //3ֱ�����
  //4������
  for( int iIdex=1;iIdex<Lsize;iIdex++)
  {
	  char cRet=left[Lsize-iIdex]-'0';
	  if(iIdex<Rsize)
	  {
       cRet-=(right[Rsize-iIdex]-'0');
	  }
	  //��ߵ�λ<�ұߣ�����ǰ��λ
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
	//���������������
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
		
		 if(_strdata[0]==bigdata._strdata [0])
		 {
			 //ͬ�Ų��ҽ��û�����
			 if(('+'==_strdata[0] && MAX_INT64 /_value>=bigdata._value)
				 ||'-'==_strdata[0] && MAX_INT64 /_value<=bigdata._value)
			 {
			   return BigData(_value*bigdata._value); 
			 }

		 }
		 else
		 {
	       //��Ų��ҽ��û�����
			if(('-'==_strdata[0]&&MIN_INT64 /_value>=bigdata._value)
				||('+'==_strdata[0]&&MIN_INT64 /_value<=bigdata._value))
			{
			  return BigData(_value*bigdata._value);
			}
		 }
	}

	//�������
	//1���������
	//2������
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
	//c.assign (n,elem)��n��elem�Ŀ�����ֵ��c
	strRet.assign (Lsize+Rsize-1,'0');//������Ҫ���ǳ�ʼ����ʱ��Ϊ�ַ�0����������0
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
//�θ�λ�����λ�����Ľ�λ
	   strRet[DataLen - offset - Rsize] += cstep;
		offset++;
	}
	return strRet;
}

 BigData BigData::operator/(const BigData& bigdata)
{
	//��������Ϊ0
	if(0==bigdata._value)
	{
	  assert(false);
	}
	//û�����
	if(!IsINT64Ooverflow()&&!bigdata.IsINT64Ooverflow())
	{
	  return BigData(_value/bigdata._value);
	}
	//���
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

	//2 ����ֵleft=right
	if(strcmp(left.c_str()+1,right.c_str()+1)==0)
	{
	//ͬ��
	   if(left[0]==right[0])
	   {
		return "1";
	   }
		//���
	   return "-1";
	}
	//3 ���right=-,+1
	    //+33333 +1
	if("+1"==right||"-1"==right)
	{
	  left[0]=csymbol;
	  return left;
	}

	string strRet;
	//string &append(int n,char c)�ڵ�ǰ�ַ�����β���n���ַ�c
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
		//ѭ�����
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
	//�ж�������
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
   //���û�����
	if(!bigdata.IsINT64Ooverflow())
	{
		os<<bigdata._value;
	}
	//������
	else
	{
		char* pdata=(char*)bigdata._strdata .c_str();//�ַ������׵�ַ
		if('+'==bigdata._strdata[0])
		  {
		   pdata++; 
		  }
		os<<pdata;
	}
	return os;
}
