//
// Created by lhy on 5/7/24.
//

#include "LCD.h"



LCD::LCD(GPIO_TypeDef* mGPIOx,uint16_t mSCL,uint16_t mSDA,uint16_t mRES,uint16_t mDC,uint16_t mCS,uint16_t mBLK,SPI_HandleTypeDef* mspiHandle) {
    spiHandle = mspiHandle;

    change = 0;



    if (spiHandle == NULL) {
        isSPI = false;
    } else {
        isSPI = true;
    }

    GPIOx = mGPIOx;

    SCL = mSCL;
    SDA = mSDA;
    RES = mRES;
    DC = mDC;
    CS = mCS;
    BLK = mBLK;
}

//初始化GPIO
void LCD::GPIO_Init(void){

}
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD::Writ_Bus(u8 dat)
{
    u8 i;
    CS_Clr();
    for(i=0;i<8;i++)
    {
        SCLK_Clr();
        if(dat&0x80)
        {
            MOSI_Set();
        }
        else
        {
            MOSI_Clr();
        }
        SCLK_Set();
        dat<<=1;
    }
    CS_Set();
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD::WR_DATA8(u8 dat,bool buf = false,u16 x = 0,u16 y = 0)
{
    if (isSPI){
        HAL_SPI_Transmit(spiHandle, &dat, 1, 1000);
    } else {
        Writ_Bus(dat);
    }
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD::WR_DATA(u16 dat,bool buf = false,u16 x = 0,u16 y = 0)
{
    static u8 data[2];
    data[0] = dat >> 8;
    data[1]  = dat & 0xff;
    if (isSPI){
        if (buf){
            buffer[y][x] = data[1]<<8|data[0];
        } else {
                HAL_SPI_Transmit(spiHandle, (u8 *) (data), 2, 1000);
        }
    } else {
        Writ_Bus(data[0]);
        Writ_Bus(data[1]);
    }
}

/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD::WR_REG(u8 dat)
{
    DC_Clr();//写命令
    if (isSPI){
        HAL_SPI_Transmit(spiHandle, &dat, 1, 1000);
    } else {
        Writ_Bus(dat);
    }
    DC_Set();//写数据
}

void LCD::Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
    if(USE_HORIZONTAL==0)
    {
        WR_REG(0x2a);//列地址设置
        WR_DATA(x1 + 2);
        WR_DATA(x2 + 2);
        WR_REG(0x2b);//行地址设置
        WR_DATA(y1 + 1);
        WR_DATA(y2 + 1);
        WR_REG(0x2c);//储存器写
    }
    else if(USE_HORIZONTAL==1)
    {
        WR_REG(0x2a);//列地址设置
        WR_DATA(x1 + 2);
        WR_DATA(x2 + 2);
        WR_REG(0x2b);//行地址设置
        WR_DATA(y1 + 1);
        WR_DATA(y2 + 1);
        WR_REG(0x2c);//储存器写
    }
    else if(USE_HORIZONTAL==2)
    {
        WR_REG(0x2a);//列地址设置
        WR_DATA(x1 + 1);
        WR_DATA(x2 + 1);
        WR_REG(0x2b);//行地址设置
        WR_DATA(y1 + 2);
        WR_DATA(y2 + 2);
        WR_REG(0x2c);//储存器写
    }
    else
    {
        WR_REG(0x2a);//列地址设置
        WR_DATA(x1 + 1);
        WR_DATA(x2 + 1);
        WR_REG(0x2b);//行地址设置
        WR_DATA(y1 + 2);
        WR_DATA(y2 + 2);
        WR_REG(0x2c);//储存器写
    }
}
void LCD::Init(void)
{
    GPIO_Init();//初始化GPIO

    RES_Clr();//复位
    HAL_Delay(100);
    RES_Set();
    HAL_Delay(100);

    BLK_Set();//打开背光
    HAL_Delay(100);

    //************* Start Initial Sequence **********//
    WR_REG(0x11); //Sleep out
    HAL_Delay(120);              //Delay 120ms
    //------------------------------------ST7735S Frame Rate-----------------------------------------//
    WR_REG(0xB1);
    WR_DATA8(0x05);
    WR_DATA8(0x3C);
    WR_DATA8(0x3C);
    WR_REG(0xB2);
    WR_DATA8(0x05);
    WR_DATA8(0x3C);
    WR_DATA8(0x3C);
    WR_REG(0xB3);
    WR_DATA8(0x05);
    WR_DATA8(0x3C);
    WR_DATA8(0x3C);
    WR_DATA8(0x05);
    WR_DATA8(0x3C);
    WR_DATA8(0x3C);
    //------------------------------------End ST7735S Frame Rate---------------------------------//
    WR_REG(0xB4); //Dot inversion
    WR_DATA8(0x03);
    //------------------------------------ST7735S Power Sequence---------------------------------//
    WR_REG(0xC0);
    WR_DATA8(0x28);
    WR_DATA8(0x08);
    WR_DATA8(0x04);
    WR_REG(0xC1);
    WR_DATA8(0XC0);
    WR_REG(0xC2);
    WR_DATA8(0x0D);
    WR_DATA8(0x00);
    WR_REG(0xC3);
    WR_DATA8(0x8D);
    WR_DATA8(0x2A);
    WR_REG(0xC4);
    WR_DATA8(0x8D);
    WR_DATA8(0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    WR_REG(0xC5); //VCOM
    WR_DATA8(0x1A);
    WR_REG(0x36); //MX, MY, RGB mode
    if(USE_HORIZONTAL==0)WR_DATA8(0x00);
    else if(USE_HORIZONTAL==1)WR_DATA8(0xC0);
    else if(USE_HORIZONTAL==2)WR_DATA8(0x70);
    else WR_DATA8(0xA0);
    //------------------------------------ST7735S Gamma Sequence---------------------------------//
    WR_REG(0xE0);
    WR_DATA8(0x04);
    WR_DATA8(0x22);
    WR_DATA8(0x07);
    WR_DATA8(0x0A);
    WR_DATA8(0x2E);
    WR_DATA8(0x30);
    WR_DATA8(0x25);
    WR_DATA8(0x2A);
    WR_DATA8(0x28);
    WR_DATA8(0x26);
    WR_DATA8(0x2E);
    WR_DATA8(0x3A);
    WR_DATA8(0x00);
    WR_DATA8(0x01);
    WR_DATA8(0x03);
    WR_DATA8(0x13);
    WR_REG(0xE1);
    WR_DATA8(0x04);
    WR_DATA8(0x16);
    WR_DATA8(0x06);
    WR_DATA8(0x0D);
    WR_DATA8(0x2D);
    WR_DATA8(0x26);
    WR_DATA8(0x23);
    WR_DATA8(0x27);
    WR_DATA8(0x27);
    WR_DATA8(0x25);
    WR_DATA8(0x2D);
    WR_DATA8(0x3B);
    WR_DATA8(0x00);
    WR_DATA8(0x01);
    WR_DATA8(0x04);
    WR_DATA8(0x13);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------//
    WR_REG(0x3A); //65k mode
    WR_DATA8(0x05);
    WR_REG(0x29); //Display on

    change = 1;



}


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD::Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i,j;
//    Address_Set(xsta, ysta, xend - 1, yend - 1);//设置显示范围
    for(i=ysta;i<yend;i++)
    {
        for(j=xsta;j<xend;j++)
        {
            WR_DATA(color, true, j, i);
        }
    }
}
void LCD::DMA_Fill() {
    Address_Set(0, 0, LCD_W-1, LCD_H-1);
    HAL_SPI_Transmit_DMA(spiHandle, (u8*)buffer, LCD_H*LCD_W*2);
}
/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD::DrawPoint(u16 x, u16 y, u16 color)
{
//    Address_Set(x, y, x, y);//设置光标位置

    WR_DATA(color, true, x, y);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD::DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if (delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线
    else {incy=-1;delta_y=-delta_y;}
    if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        DrawPoint(uRow, uCol, color);//画点
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD::DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    DrawLine(x1, y1, x2, y1, color);
    DrawLine(x1, y1, x1, y2, color);
    DrawLine(x1, y2, x2, y2, color);
    DrawLine(x2, y1, x2, y2, color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void LCD::Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
    int a,b;
    a=0;b=r;
    while(a<=b)
    {
        DrawPoint(x0 - b, y0 - a, color);             //3
        DrawPoint(x0 + b, y0 - a, color);             //0
        DrawPoint(x0 - a, y0 + b, color);             //1
        DrawPoint(x0 - a, y0 - b, color);             //2
        DrawPoint(x0 + b, y0 + a, color);             //4
        DrawPoint(x0 + a, y0 - b, color);             //5
        DrawPoint(x0 + a, y0 + b, color);             //6
        DrawPoint(x0 - b, y0 + a, color);             //7
        a++;
        if((a*a+b*b)>(r*r))//判断要画的点是否过远
        {
            b--;
        }
    }
}


/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    while(*s!=0)
    {
        if(sizey==12) ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        else if(sizey==16) ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        else if(sizey==24) ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        else if(sizey==32) ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        else return;
        s+=2;
        x+=sizey;
    }
}


/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i,j,m=0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

    HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
        {
            Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))WR_DATA(fc, true, x, y);
                        else WR_DATA(bc, true, x, y);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))	DrawPoint(x, y, fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i,j,m=0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
        {
            Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))WR_DATA(fc, true, x, y);
                        else WR_DATA(bc, true, x, y);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))	DrawPoint(x, y, fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i,j,m=0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
        {
            Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))WR_DATA(fc, true, x, y);
                        else WR_DATA(bc, true, x, y);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))	DrawPoint(x, y, fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i,j,m=0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))WR_DATA(fc, true, x, y);
                        else WR_DATA(bc, true, x, y);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))	DrawPoint(x, y, fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 temp,sizex,t,m=0;
    u16 i,TypefaceNum;//一个字符所占字节大小
    u16 x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //得到偏移后的值
//    Address_Set(x, y, x + sizex - 1, y + sizey - 1);  //设置光标位置
    for(i=0;i<TypefaceNum;i++)
    {
        if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
        else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
        else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
        else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//非叠加模式
            {
                if(temp&(0x01<<t))WR_DATA(fc, true, x, y);
                else WR_DATA(bc, true, x, y);
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
            else//叠加模式
            {
                if(temp&(0x01<<t))DrawPoint(x, y, fc);//画一个点
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD::ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    while(*p!='\0')
    {
        ShowChar(x, y, *p, fc, bc, sizey, mode);
        x+=sizey/2;
        p++;
    }
}

/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 LCD::mypow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD::ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey)
{
    u8 t,temp;
    u8 enshow=0;
    u8 sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            }else enshow=1;

        }
        ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD::ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey)
{
    u8 t,temp,sizex;
    u16 num1;
    sizex=sizey/2;
    num1=num*100;
    for(t=0;t<len;t++)
    {
        temp=(num1/mypow(10,len-t-1))%10;
        if(t==(len-2))
        {
            ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len+=1;
        }
        ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD::ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[])
{
    u16 i,j;
    u32 k=0;
//    Address_Set(x, y, x + length - 1, y + width - 1);
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            static u16 color;
            color = pic[k * 2] << 8 | pic[k * 2+1];
            WR_DATA(color, true, x+j, y+i);

//            WR_DATA8(pic[k * 2], true, x, y);
//            WR_DATA8(pic[k * 2 + 1], true, x, y);

            k++;
        }
    }
}


