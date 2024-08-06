#include "Servo.h"
#include "myusart.h"
#include "fashion_star_uart_servo.h"
#include "mymath.h"

#define value0 value1 - 89 // �ڶ���������0�Ŷ����׼ֵ
#define value1 0		   // 0�Ŷ����׼ֵ�������Ǹ���
#define value2 0		   // 1�Ŷ����׼ֵ
#define value3 0		   // 2�Ŷ����׼ֵ
#define value4 0		   // 3�Ŷ����׼ֵ
#define value5 0		   // 4�Ŷ����׼ֵ
#define value6 0		   // 5�Ŷ����׼ֵ
#define intervals 85	   // 0�Ŷ��ת��
#define interval 130	   // ����Ƕȵ���ת���ڣ�ms  150
#define t_acc 100		   // ����ʱ�� ms
#define t_dec 100		   // ����ʱ�� ms
#define power 0			   // ���ִ�й��ʣ�Ĭ��Ϊ0
#define wait 0			   // �Ƿ�Ϊ����ʽ  0--������

_Robotic_arm_t robotic_arm;
_Servo_t Servo;

void Robotic_arm_Init(_Robotic_arm_t *r_arm)
{
    r_arm->Vehicle_height = 0.00;
    r_arm->Length_Servo = 3.5;
    r_arm->L_Servo_zero_one = 0.00;
    r_arm->L_Servo_one_two = 13.70;
    r_arm->L_Servo_two_three = 14.00;
    r_arm->L_Servo_three_top = 6.51;
    r_arm->MAX_high = r_arm->Length_Servo + r_arm->L_Servo_one_two + r_arm->L_Servo_two_three + r_arm->L_Servo_three_top;
    r_arm->MAX_len = r_arm->L_Servo_one_two + r_arm->L_Servo_two_three + r_arm->L_Servo_three_top;
    r_arm->Next_max_len = r_arm->L_Servo_one_two + r_arm->L_Servo_two_three;
}

/******************************************************4�ᴮ����е��������㷨***********************************************************/
/*************ע�⣡���������ᵽ��ƫ��Ƕȶ�����ʵ�ʽǶȣ�Ҫ�����Ƕ�ת�����ܵõ�ʵ��ֵ**************************/
float _degree_convert(int temp, float angle) // �Ƕ�ת����ֵ
{
    float res = 0;
	if (temp == 0)
	{
		res = 90 - angle;
	}
	if (temp == 1 || temp == 2)
	{
		res = 180 - angle;
	}
	if (temp == 3)
	{
		res = angle - 90;
	}
	return res;
}

float out_of_range(_Robotic_arm_t *r_arm, uint16_t length, uint16_t height) // ���꣨�Ƕȣ���Χ
{
    if (height < r_arm->MAX_high && length < r_arm->MAX_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

float _calculatr_j0(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // ���0��ƫ��Ƕ�
{
    float j0 = 0;
    Servo->length = rounded(sqrt(pow(y, 2) + pow(x, 2)) - r_arm->L_Servo_zero_one); // ��е��x��ͶӰ����
    if (Servo->length == 0)
    {
        j0 = 0;
    }
    else
    {
        j0 = arctan(y, x);
        Servo->hight = r_arm->Vehicle_height + r_arm->Length_Servo - z;
    }
    return j0;
}

float _calculatr_j2(_Robotic_arm_t *r_arm, float temp, float d2) // ���2��ƫ��Ƕ�
{
    float j2 = 0;
    if (temp > d2)
    {
        j2 = rounded(arccos((pow(r_arm->L_Servo_one_two, 2) + pow(r_arm->L_Servo_two_three, 2) - pow(d2, 2)) / (2 * r_arm->L_Servo_one_two * r_arm->L_Servo_two_three)));
    }
    else if (absd(temp - d2) < 1.0f)
    {
        j2 = 180;
    }
    return j2;
}
/******************************************����˶���ʽ***************************************************/
void backward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // �˶�ѧ������
{
    float temp, d1, d2;
    float j0, j1, j2 = 0;

    if (out_of_range(&robotic_arm, Servo->length, Servo->hight) == 1) // �жϽǶ��Ƿ����趨��Χ��
    {
        j0 = _calculatr_j0(Servo, r_arm, x, y, z);
        temp = Servo->length - r_arm->L_Servo_three_top;                                                                                                                                        // ��е���ڸ�����������x-y����ϵ�ϵ�ͶӰ���� - ���Ŷ����צ�˵ĳ���
        d1 = sqrt(pow(temp, 2) + pow(z, 2));                                                                                                                                                    // ���Ŷ������Ŷ���ڵ���ͶӰ��֮��Ĵ�ֱ����
        d2 = sqrt(pow(r_arm->Vehicle_height + r_arm->Length_Servo - z, 2) + pow(temp, 2));                                                                                                      // ���Ŷ����һ�Ŷ��֮��Ĵ�ֱ����
        j1 = rounded(arcsin((d1 / d2) * sinx(arctan(temp, z))) + arccos((pow(r_arm->L_Servo_one_two, 2) + pow(d2, 2) - pow(r_arm->L_Servo_two_three, 2)) / (2 * r_arm->L_Servo_one_two * d2))); // ����1,2����y��֮��ļн�
        j2 = _calculatr_j2(r_arm, r_arm->Next_max_len, d2);

        if (Servo->x < 0)
		{
			Servo->angle_zero = -rounded(_degree_convert(0, j0));
		}
		else
		{
			Servo->angle_zero = rounded(_degree_convert(0, j0));
		}

		Servo->angle_one = rounded(_degree_convert(1, j1));
		Servo->angle_two = rounded(_degree_convert(2, j2));
		Servo->angle_three = rounded(_degree_convert(3, Servo->angle_one + Servo->angle_two));
    }
}
/******************************************�����˶���ʽ***************************************************/
void forward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm) // �����˶���ʽ���������������ĽǶ�ֵ�Ƿ���ȷ
{
	float length, height; // length--��е��x��ͶӰ        height--��е��y��ͶӰ
	float x = 0.0f, y = 0.0f, z = 0.0f;

	length = rounded(r_arm->L_Servo_one_two * sinx(Servo->angle_one) + r_arm->L_Servo_two_three * sinx(Servo->angle_one + Servo->angle_two) + r_arm->L_Servo_three_top);
	height = absd(absd(r_arm->L_Servo_one_two * cosx(Servo->angle_one)) - absd(r_arm->L_Servo_two_three * cosx(Servo->angle_one + Servo->angle_two)));
	z = r_arm->Vehicle_height + rounded(height) + r_arm->Length_Servo;

	if (Servo->x < 0) //   �ж����õ�x��������ֵ����ֵ
	{
		x = -round(length * rounded(cosx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(cosx(Servo->angle_zero)));
		y = -round(length * rounded(sinx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(sinx(Servo->angle_zero)));
	}
	else
	{
		x = round(length * rounded(cosx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(cosx(Servo->angle_zero)));
		y = round(length * rounded(sinx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(sinx(Servo->angle_zero)));
	}
}

void task(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // �������������ֱ�ӵ��ô˺���
{
    Servo->x = x;
    if (x < 0)
        x = absd(x);

    backward_kinematics(Servo, r_arm, x, y, z);
}

/******************************************д���˶���ʽ***************************************************/
void set_serveo_angle(uint8_t id, _Servo_t *Servo) // ����ֵ���ƶ���˶�
{

	if (id == 0)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value1 + Servo->angle_zero, intervals, t_acc, t_dec, power, wait);
	}
	if (id == 1)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value2 + Servo->angle_one, interval, t_acc, t_dec, power, wait);
	}
	if (id == 2)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value3 + Servo->angle_two, interval, t_acc, t_dec, power, wait);
	}
	if (id == 3)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value4 + Servo->angle_three, interval, t_acc, t_dec, power, wait);
	}
	if (id == 6)
	{
		FSUS_SetServoAngleByVelocity(&uart4, 0, value0 + Servo->angle_zero, intervals, t_acc, t_dec, power, wait);
	}
}

void set_serveo_angles(uint8_t id, float angle) // �ֶ����ڶ���Ƕ��˶�
{
	if (id == 0)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value1 + angle, intervals, t_acc, t_dec, power, wait);
	}
	if (id == 1)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value2 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 2)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value3 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 3)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value4 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 4)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value5 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 5)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value6 + angle, interval + 30, t_acc, t_dec, power, wait);
	}
}


