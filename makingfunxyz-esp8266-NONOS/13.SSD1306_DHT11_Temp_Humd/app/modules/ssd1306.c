/*
 * MIT License
 *
 * Copyright (c) 2018 imliubo
 *
 * Github  https://github.com/imliubo
 * Website https://www.makingfun.xyz
 * Zhihu   https://www.zhihu.com/people/MAKINGFUNXYZ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "modules/ssd1306.h"

#if defined(SSD1306_USE_I2C)
int ICACHE_FLASH_ATTR
HAL_I2C_Mem_Write( uint16_t DevAddress, uint16_t MemAddress, uint8_t pData, uint16_t Size )
{
    i2c_master_start();
    i2c_master_writeByte( DevAddress );
    if ( !i2c_master_checkAck() )
    {
        i2c_master_stop();
        return(0);
    }

    i2c_master_writeByte( MemAddress );
    if ( !i2c_master_checkAck() )
    {
        i2c_master_stop();
        return(0);
    }

    i2c_master_writeByte( pData );
    if ( !i2c_master_checkAck() )
    {
        i2c_master_stop();
        return(0);
    }

    i2c_master_stop();
    return(1);
}


void ssd1306_Reset( void )
{
    /* for I2C - do nothing */
}


/* Send a byte to the command register */
void ssd1306_WriteCommand( uint8_t byte )
{
    HAL_I2C_Mem_Write( SSD1306_I2C_ADDR, 0x00, byte, 1 );
}


/* Send data */
void ssd1306_WriteData( uint8_t buffer, size_t buff_size )
{
    HAL_I2C_Mem_Write( SSD1306_I2C_ADDR, 0x40, buffer, buff_size );
}

#elif defined(SSD1306_USE_SPI)

#else
#error "You should define SSD1306_USE_SPI or SSD1306_USE_I2C macro"
#endif


/* Screenbuffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Screen object */
static SSD1306_t SSD1306;

/* Initialize the oled screen */
void ssd1306_Init( void )
{
    /* Reset OLED */
    ssd1306_Reset();

    /* Wait for the screen to boot */
    os_delay_us( 60000 );
    os_delay_us( 40000 );

/*     // Init OLED */
    ssd1306_WriteCommand( 0xAE );   /* display off */

    ssd1306_WriteCommand( 0x20 );   /* Set Memory Addressing Mode */
    ssd1306_WriteCommand( 0x10 );   /*
                                     * 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode;
                                     * 10,Page Addressing Mode (RESET); 11,Invalid
                                     */

    ssd1306_WriteCommand( 0xB0 );   /* Set Page Start Address for Page Addressing Mode,0-7 */

#ifdef SSD1306_MIRROR_VERT
    ssd1306_WriteCommand( 0xC0 );   /* Mirror vertically */
#else
    ssd1306_WriteCommand( 0xC8 );   /* Set COM Output Scan Direction */
#endif

    ssd1306_WriteCommand( 0x00 );   /* ---set low column address */
    ssd1306_WriteCommand( 0x10 );   /* ---set high column address */

    ssd1306_WriteCommand( 0x40 );   /* --set start line address - CHECK */

    ssd1306_WriteCommand( 0x81 );   /* --set contrast control register - CHECK */
    ssd1306_WriteCommand( 0xFF );

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_WriteCommand( 0xA0 );   /* Mirror horizontally */
#else
    ssd1306_WriteCommand( 0xA1 );    /*--set segment re-map 0 to 127 - CHECK */
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd1306_WriteCommand( 0xA7 );   /* --set inverse color */
#else
    ssd1306_WriteCommand( 0xA6 );   /* --set normal color */
#endif

    ssd1306_WriteCommand( 0xA8 );   /* --set multiplex ratio(1 to 64) - CHECK */
    ssd1306_WriteCommand( 0x3F );   /*  */

    ssd1306_WriteCommand( 0xA4 );   /* 0xa4,Output follows RAM content;0xa5,Output ignores RAM content */

    ssd1306_WriteCommand( 0xD3 );   /* -set display offset - CHECK */
    ssd1306_WriteCommand( 0x00 );   /* -not offset */

    ssd1306_WriteCommand( 0xD5 );   /* --set display clock divide ratio/oscillator frequency */
    ssd1306_WriteCommand( 0xF0 );   /* --set divide ratio */

    ssd1306_WriteCommand( 0xD9 );   /* --set pre-charge period */
    ssd1306_WriteCommand( 0x22 );   /*  */

    ssd1306_WriteCommand( 0xDA );   /* --set com pins hardware configuration - CHECK */
    ssd1306_WriteCommand( 0x12 );

    ssd1306_WriteCommand( 0xDB );   /* --set vcomh */
    ssd1306_WriteCommand( 0x20 );   /* 0x20,0.77xVcc */

    ssd1306_WriteCommand( 0x8D );   /* --set DC-DC enable */
    ssd1306_WriteCommand( 0x14 );   /*  */
    ssd1306_WriteCommand( 0xAF );   /* --turn on SSD1306 panel */


    /* Clear screen */
    ssd1306_Fill( Black );

    /* Flush buffer to screen */
    ssd1306_UpdateScreen();

    /* Set default values for screen object */
    SSD1306.CurrentX    = 0;
    SSD1306.CurrentY    = 0;

    SSD1306.Initialized = 1;
}


/* Fill the whole screen with the given color */
void ssd1306_Fill( SSD1306_COLOR color )
{
    /* Set memory */
    uint32_t i;

    for ( i = 0; i < sizeof(SSD1306_Buffer); i++ )
    {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}


/* Write the screenbuffer with changed to the screen */
void ssd1306_UpdateScreen( void )
{
    uint8_t i, j;
    for ( i = 0; i < 8; i++ )
    {
        ssd1306_WriteCommand( 0xB0 + i );
        ssd1306_WriteCommand( 0x00 );
        ssd1306_WriteCommand( 0x10 );
        for ( int j = 0; j < 128; j++ )
        {
            /* code */
            ssd1306_WriteData( SSD1306_Buffer[j + SSD1306_WIDTH * i], SSD1306_WIDTH );
        }
    }
}


/*
 *    Draw one pixel in the screenbuffer
 *    X => X Coordinate
 *    Y => Y Coordinate
 *    color => Pixel color
 */
void ssd1306_DrawPixel( uint8_t x, uint8_t y, SSD1306_COLOR color )
{
    if ( x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT )
    {
        /* Don't write outside the buffer */
        return;
    }

    /* Check if pixel should be inverted */
    if ( SSD1306.Inverted )
    {
        color = (SSD1306_COLOR) !color;
    }

    /* Draw in the right color */
    if ( color == White )
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8) );
    }
}


/*
 * Draw 1 char to the screen buffer
 * ch         => char om weg te schrijven
 * Font     => Font waarmee we gaan schrijven
 * color     => Black or White
 */
char ssd1306_WriteChar( char ch, FontDef Font, SSD1306_COLOR color )
{
    uint32_t i, b, j;

    /* Check remaining space on current line */
    if ( SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
         SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight) )
    {
        /* Not enough space on current line */
        return(0);
    }

    /* Use the font to write */
    for ( i = 0; i < Font.FontHeight; i++ )
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for ( j = 0; j < Font.FontWidth; j++ )
        {
            if ( (b << j) & 0x8000 )
            {
                ssd1306_DrawPixel( SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color );
            } else {
                ssd1306_DrawPixel( SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) !color );
            }
        }
    }

    /* The current space is now taken */
    SSD1306.CurrentX += Font.FontWidth;

    /* Return written char for validation */
    return(ch);
}

/*
 * Draw 1 Chinese char to the screen buffer
 * ch         => char 
 * color     => Black or White
 */
char ssd1306_WriteZhChar( signed char ch[], SSD1306_COLOR color ){


    uint32_t  b, j, k, data[32];

    /* Check remaining space on current line */
    if ( SSD1306_WIDTH <= (SSD1306.CurrentX + 16) ||
         SSD1306_HEIGHT <= (SSD1306.CurrentY + 16) )
    {
        /* Not enough space on current line */
        return(0);
    }

    for (int i = 0; i < 20; ++i)
    {
        if ((ZhFont16x16[i].Index[0] == ch[0]) && (ZhFont16x16[i].Index[1] == ch[1]) && (ZhFont16x16[i].Index[2] == ch[2]) )
        {
            for (int z = 0; z < ZH_CN_HEIGHT_WIDTH; z++)
            {
                data[z] = ZhFont16x16[i].Msk[z];

            }
        }
    }

    for ( int i = 0; i < ZH_CN_HEIGHT_WIDTH; i++ )
    {

        b = data[i];
        for ( j = 0; j < ZH_CN_HEIGHT_WIDTH; j++ )
        {
            //os_printf("Font: %d\n",Font.FontWidth);
            if ( (b << j) & 0x8000 )
            {
                ssd1306_DrawPixel( SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color );
            } else {
                ssd1306_DrawPixel( SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) !color );
            }
        }
    }

    /* The current space is now taken */
    SSD1306.CurrentX += ZH_CN_HEIGHT_WIDTH;

    /* Return written char for validation */
    return(*ch);   
}


/* Write full string to screenbuffer */
char ssd1306_WriteString( char* str, FontDef Font, SSD1306_COLOR color )
{
    /* Write until null-byte */
    while ( *str )
    {
        if ( ssd1306_WriteChar( *str, Font, color ) != *str )
        {
            /* Char could not be written */
            
            return(*str);
        }

        /* Next char */
        str++;
    }

    /* Everything ok */
    return(*str);
}

/* Write full Chinese string to screenbuffer */
char ssd1306_WriteZhString( signed char *str, SSD1306_COLOR color ){

    /* Write until null-byte */
    while ( *str )
    {
        ssd1306_WriteZhChar( (signed char *)str, color );
        /* Next char */
        str = str + 3;

    }
    /* Everything ok */
    return(*str);

}


/* Position the cursor */
void ssd1306_SetCursor( uint8_t x, uint8_t y )
{
    SSD1306.CurrentX    = x;
    SSD1306.CurrentY    = y;
}


