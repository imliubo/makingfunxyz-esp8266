#include <stdint.h>

#ifndef __SSD1306_FONTS_H__
#define __SSD1306_FONTS_H__

typedef struct
{
	const uint8_t	FontWidth;      /*!< Font width in pixels */
	uint8_t			FontHeight;     /*!< Font height in pixels */
	const uint16_t	*data;          /*!< Pointer to data font data array */
} FontDef;

#define ZH_CN_HEIGHT_WIDTH  16
#define ZH_CN_NUMBER		20

typedef struct                     // 汉字字模数据结构
{
       signed char Index[3];       // 汉字内码索引
       const uint16_t Msk[16];     // 点阵码数据

}ZhFont_GB16x16;


extern FontDef	Font_7x10;
extern FontDef	Font_11x18;
extern FontDef	Font_16x26;
extern ZhFont_GB16x16 ZhFont16x16[];

#endif /* __SSD1306_FONTS_H__ */