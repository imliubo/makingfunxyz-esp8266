#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "c_types.h"
#include "ets_sys.h"
#include "osapi.h"
#include "driver/i2c_master.h"
#include "modules/ssd1306_fonts.h"


#define SSD1306_USE_I2C
//#define SSD1306_USE_SPI

/* vvv I2C config vvv */

#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR 		0x78
//#define SSD1306_I2C_ADDR        0x7a
#endif

/* ^^^ I2C config ^^^ */

/* vvv SPI config vvv */

#ifndef SSD1306_SPI_PORT
#define SSD1306_SPI_PORT
#endif

#ifndef SSD1306_CS_Port
#define SSD1306_CS_Port
#endif
#ifndef SSD1306_CS_Pin
#define SSD1306_CS_Pin
#endif

#ifndef SSD1306_DC_Port
#define SSD1306_DC_Port
#endif
#ifndef SSD1306_DC_Pin
#define SSD1306_DC_Pin
#endif

#ifndef SSD1306_Reset_Port
#define SSD1306_Reset_Port
#endif
#ifndef SSD1306_Reset_Pin
#define SSD1306_Reset_Pin
#endif

/* ^^^ SPI config ^^^ */

/* SSD1306 OLED height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 130
#endif

/*
 * some LEDs don't display anything in first two columns
 * #define SSD1306_WIDTH           130
 */


/* Enumeration for screen colors */
typedef enum {
	Black	= 0x00, /* Black color, no pixel */
	White	= 0x01  /* Pixel is set. Color depends on OLED */
} SSD1306_COLOR;

/* Struct to store transformations */
typedef struct {
	uint16_t	CurrentX;
	uint16_t	CurrentY;
	uint8_t		Inverted;
	uint8_t		Initialized;
} SSD1306_t;

/* Procedure definitions */
void ssd1306_Init( void );


void ssd1306_Fill( SSD1306_COLOR color );


void ssd1306_UpdateScreen( void );


void ssd1306_DrawPixel( uint8_t x, uint8_t y, SSD1306_COLOR color );


char ssd1306_WriteChar( char ch, FontDef Font, SSD1306_COLOR color );


char ssd1306_WriteZhChar( signed char ch[], SSD1306_COLOR color );


char ssd1306_WriteString( char* str, FontDef Font, SSD1306_COLOR color );


char ssd1306_WriteZhString( signed char *str, SSD1306_COLOR color );


char ssd1306_DrawPicture(  uint8_t height, uint8_t width,  const uint16_t picBuffer[], SSD1306_COLOR color);


void ssd1306_SetCursor( uint8_t x, uint8_t y );


/* Low-level procedures */
void ssd1306_Reset( void );


void ssd1306_WriteCommand( uint8_t byte );


void ssd1306_WriteData( uint8_t buffer, size_t buff_size );


#endif /* __SSD1306_H__ */