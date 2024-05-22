//
// Created by lhy on 5/7/24.
//

#ifndef LCD_LCD_H
#define LCD_LCD_H


#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"
#include "gpio.h"
#include "type.h"
#include "lcdfont.h"

#ifdef __cplusplus
}
#endif

class LCD {
private:

    bool change;

    SPI_HandleTypeDef* spiHandle;

    GPIO_TypeDef* GPIOx;

    uint16_t SCL;
    uint16_t SDA;
    uint16_t CS;
    uint16_t BLK;
    uint16_t RES;
    uint16_t DC;

    bool isSPI;

    void SCLK_Clr(){HAL_GPIO_WritePin(GPIOx,SCL,GPIO_PIN_RESET);}
    void SCLK_Set(){HAL_GPIO_WritePin(GPIOx,SCL,GPIO_PIN_SET);}
    void MOSI_Clr(){HAL_GPIO_WritePin(GPIOx,SDA,GPIO_PIN_RESET);}
    void MOSI_Set(){HAL_GPIO_WritePin(GPIOx,SDA,GPIO_PIN_SET);}
    void RES_Clr() {HAL_GPIO_WritePin(GPIOx,RES,GPIO_PIN_RESET);}
    void RES_Set() {HAL_GPIO_WritePin(GPIOx,RES,GPIO_PIN_SET);}
    void DC_Clr()  {HAL_GPIO_WritePin(GPIOx,DC,GPIO_PIN_RESET);}
    void DC_Set()  {HAL_GPIO_WritePin(GPIOx,DC,GPIO_PIN_SET);}
    void CS_Clr()  {HAL_GPIO_WritePin(GPIOx,CS,GPIO_PIN_RESET);}
    void CS_Set()  {HAL_GPIO_WritePin(GPIOx,CS,GPIO_PIN_SET);}
    void BLK_Clr() {HAL_GPIO_WritePin(GPIOx,BLK,GPIO_PIN_RESET);}
    void BLK_Set() {HAL_GPIO_WritePin(GPIOx,BLK,GPIO_PIN_SET);}

    void GPIO_Init(void);//初始化GPIO
    void Writ_Bus(u8 dat);//模拟SPI时序
    void WR_DATA8(u8 dat, bool buf,u16 x,u16 y);//写入一个字节

    void WR_REG(u8 dat);//写入一个指令


public:
    u16 buffer[LCD_H][LCD_W];

    LCD(GPIO_TypeDef* mGPIOx,uint16_t mSCL,uint16_t mSDA,uint16_t mRES,uint16_t mDC,uint16_t mCS,uint16_t mBLK, SPI_HandleTypeDef* mspiHandle);

    void DMA_Fill();//写入一个字节
    void WR_DATA(u16 dat,bool buf,u16 x,u16 y);//写入两个字节
    void Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);//设置坐标函数

    void Init(void);//LCD初始化
    void Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);//指定区域填充颜色
    void DrawPoint(u16 x, u16 y, u16 color);//在指定位置画一个点
    void DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);//在指定位置画一条线
    void DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);//在指定位置画一个矩形
    void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

    void ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);//显示汉字串
    void ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);//显示单个12x12汉字
    void ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);//显示单个16x16汉字
    void ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);//显示单个24x24汉字
    void ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);//显示单个32x32汉字

    void ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode);//显示一个字符
    void ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode);//显示字符串
    u32 mypow(u8 m,u8 n);//求幂
    void ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey);//显示整数变量
    void ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey);//显示两位小数变量

    void ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]);//显示图片


};


#endif //LCD_LCD_H
