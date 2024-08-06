#include "mymath.h"

#define PI 3.14159
float rounded(float angle) //��������ȡ��λС��
{
	float temp;
	temp=(int)((angle * 100) + 0.5) / 100.0;
	return temp;
}

float absd(float temp)     //����ֵ
{
	if(temp<0)
	{
		return -temp;
	}
	else
	{
		return temp;
	}
		
}

float arctan(uint16_t x,uint16_t y)   //arctan����ֵת���Ƕ�ֵ
{ 
	float temp;
	temp=atan2(x,y)*180/PI;
	return temp;
}

float arcsin(float temp)           //arcsin����ֵת���Ƕ�ֵ
{
	float angle;
	angle=asin(temp)*180/PI;
	return angle;
	
}

float arccos(float temp)          //arccos����ֵת���Ƕ�ֵ
{
	float angle;
	angle=acos(temp)*180/PI;
	return angle;
	
}

float sinx(float temp)            //sin����ֵת���Ƕ�ֵ
{
	float angle;
	angle=sin(temp*PI/180);
	return angle;
}
float cosx(float temp)          //cos����ֵת���Ƕ�ֵ
{
	float angle;
	angle=cos(temp*PI/180);
	return angle;
}
