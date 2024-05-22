//
// Created by lhy on 5/7/24.
//
#define __USING_LVGL 0

#if __USING_LVGL

#include "CCpp.h"
extern LCD lcd;

void lvgl_LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, lv_color_t *color)
{

    uint32_t y=0;
    u16 height, width;
    width = ex - sx + 1;            //得到填充的宽度
    height = ey - sy + 1;           //高度

    lcd.Address_Set(sx,sy,ex,ey);


    for(y = 0; y <width*height; y++)
    {
        lcd.WR_DATA(color->full);
        color++;
    }
}
#endif
