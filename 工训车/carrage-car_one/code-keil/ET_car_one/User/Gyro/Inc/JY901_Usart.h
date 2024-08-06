#ifndef _MY_USART_H
#define _MY_USART_H

#include "JY901.h"
#include "Defines.h"

#include "usart.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

/* �ض���洢�����Ƿ��͵�3�����ݵĽṹ�� */
// ŷ����/�Ƕ�
typedef struct
{
	float angle[3];
} Angle;

// ���ٶ�
typedef struct
{
	float a[3];
} Acc;

// ���ٶ�
typedef struct
{
	float w[3];
} SGyro;

typedef struct
{
	uint8_t Rx_flag;				// ������ɱ�־
	uint8_t Rx_len;					// ���ճ���
	uint8_t frame_head;				// ֡ͷ
	uint8_t RxBuffer[RXBUFFER_LEN]; // ���ݴ洢
	Angle angle;
	Acc acc;
	SGyro w;
} User_USART;

extern User_USART JY901_data;

void User_USART_Init(User_USART *Data);
void JY901_process(void);

#endif
