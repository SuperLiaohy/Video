//
// Created by lhy on 5/7/24.
//

#define __USING_LVGL 0

#if __USING_LVGL

#ifndef LCD_CCPP_H
#define LCD_CCPP_H



#include "LCD.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
void lvgl_LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, lv_color_t *color);

#ifdef __cplusplus
}
#endif

#endif //LCD_CCPP_H


#endif