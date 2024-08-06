#ifndef __LCD_H__
#define __LCD_H__

#include "stdlib.h"
#include "main.h"

// LCD重要参数集
typedef struct
{
	uint16_t width;	  // LCD 宽度
	uint16_t height;  // LCD 高度
	uint16_t id;	  // LCD ID
	uint8_t dir;	  // 横屏还是竖屏控制：0，竖屏；1，横屏。
	uint16_t wramcmd; // 开始写gram指令
	uint16_t setxcmd; // 设置x坐标指令
	uint16_t setycmd; // 设置y坐标指令
} _lcd_dev;

// LCD参数
extern _lcd_dev lcddev; // 管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////
#define USE_HORIZONTAL 0 // 定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////
// 定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

// TFTLCD部分外要调用的函数
extern uint16_t POINT_COLOR; // 默认红色
extern uint16_t BACK_COLOR;	 // 背景颜色.默认为白色

// QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度

#define LCD_LED BL_GPIO_Port->BSRR = BL_Pin // LCD背光

// 如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法

// GPIO置位（拉高）
#define LCD_CS_SET CS_GPIO_Port->BSRR = CS_Pin	  // 片选端口
#define LCD_RS_SET DC_GPIO_Port->BSRR = DC_Pin	  // 数据/命令
#define LCD_RST_SET RES_GPIO_Port->BSRR = RES_Pin // 复位

// GPIO复位（拉低）
#define LCD_CS_CLR CS_GPIO_Port->BSRR = (uint32_t)CS_Pin << 16U	   // 片选端口
#define LCD_RS_CLR DC_GPIO_Port->BSRR = (uint32_t)DC_Pin << 16U	   // 数据/命令
#define LCD_RST_CLR RES_GPIO_Port->BSRR = (uint32_t)RES_Pin << 16U // 复位

// 画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430	 // 灰色
// GUI颜色

#define DARKBLUE 0X01CF	 // 深蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
#define GRAYBLUE 0X5458	 // 灰蓝色
// 以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F // 浅绿色
#define LIGHTGRAY 0XEF5B  // 浅灰色(PANNEL)
#define LGRAY 0XC618	  // 浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651 // 浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12	 // 浅棕蓝色(选择条目的反色)

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x, uint16_t y);		// 画点

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);

void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);

void LCD_SetParam(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction);

#endif
