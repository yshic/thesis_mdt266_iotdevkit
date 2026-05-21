/**
 * @file       drv_ssd1306.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-07-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for driver ssd1306
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRV_SSD1306_H
#define __DRV_SSD1306_H

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"

#include "bsp_i2c.h"
#include "fonts.h"


/* Public defines ----------------------------------------------------- */
#define SSD1306_USE_I2C
#define SSD1306_I2C_ADDR       (0x3C << 1)
#define SSD1306_X_OFFSET_LOWER 0 /**/
#define SSD1306_X_OFFSET_UPPER 0 /**/
#define SSD1306_WIDTH          128
#define SSD1306_HEIGHT         64
#define SSD1306_BUFFER_SIZE    (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6X8
#define SSD1306_INCLUDE_FONT_7X10
#define SSD1306_INCLUDE_FONT_11X18
#define SSD1306_INCLUDE_FONT_16X26
#define SSD1306_INCLUDE_FONT_16X24
#define SSD1306_INCLUDE_FONT_16X15

/* Public enumerate/structure ----------------------------------------- */

/**
 * @brief Colors
 */
typedef enum
{
  DRV_SSD1306_COLOR_BLACK, /* Black color, no pixel */
  DRV_SSD1306_COLOR_WHITE  /* Pixel is set*/
} drv_ssd1306_color_t;

/**
 * @brief Display screen state
 */
typedef enum
{
  DRV_SSD1306_DISPLAY_OFF, /* Turn off the screen */
  DRV_SSD1306_DISPLAY_ON   /* Turn on the screen */
} drv_ssd1306_display_t;

/**
 * @brief Function status
 */
typedef enum
{
  DRV_SSD1306_OK,  /* No error */
  DRV_SSD1306_ERR, /* Generic error */
  DRV_SSD1306_BUSY /* Device is busy */
} drv_ssd1306_error_t;

/**
 * @brief Struct to store transformations
 */
typedef struct
{
  uint16_t current_x;   /**< x coordinate */
  uint16_t current_y;   /**< y coordinate */
  uint8_t  initialized; /**< Flag for driver init */
  uint8_t  display_on;  /**< Flag for display on/off */
} drv_ssd1306_t;

/**
 * @brief Struct for font
 */
typedef struct
{
  const uint8_t         width;      /**< Font width in pixels */
  const uint8_t         height;     /**< Font height in pixels */
  const uint16_t *const data;       /**< Pointer to font data array */
  const uint8_t *const  char_width; /**< Proportional character width in pixels */
} drv_ssd1306_font_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief   Inititate the ssd1306
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_init(void);

/**
 * @brief   Send a byte to the command register
 *
 * @param[in]   byte        byte to be send
 *
 * @return      void
 */
drv_ssd1306_error_t drv_ssd1306_write_command(uint8_t byte);

/**
 * @brief   Send data
 *
 * @param[in]   byte        byte to be send
 *
 * @return      void
 */
drv_ssd1306_error_t drv_ssd1306_write_data(uint8_t *buffer, size_t buff_size);

/**
 * @brief	Fill the screen with a given color (Black/White)
 *
 * @param[in] 	color									Color to fill the screen
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_fill(drv_ssd1306_color_t color);

/**
 * @brief		Update the screen
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_update_screen(void);

/**
 * @brief  Turn on/off the screen
 *
 * @param[in]		status 								The status of the screen (on/off)
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_set_display(drv_ssd1306_display_t status);

/**
 * @brief  Set the contrast of the SSD1306 display.
 *
 * @param[in]     value    The contrast value to set.
 *
 * @attention  This function sends the contrast control command to the display.
 *
 * @return      void
 */
drv_ssd1306_error_t drv_ssd1306_set_contrast(uint8_t value);

/**
 * @brief  Set the position of the cursor
 *
 * @param[in]     x       The x-coordinate.
 * @param[in]     y       The y-coordinate.
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_set_cursor(uint8_t x, uint8_t y);

/**
 * @brief  Draw a pixel on the SSD1306 display.
 *
 * @param[in]     x       The x-coordinate of the pixel.
 * @param[in]     y       The y-coordinate of the pixel.
 * @param[in]     color   The color of the pixel (WHITE or BLACK).
 *
 * @attention		This function does not write outside the buffer.
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_draw_pixel(uint8_t x, uint8_t y, drv_ssd1306_color_t color);

/**
 * @brief  Write a character on the SSD1306 display.
 *
 * @param[in]     ch       The character to write.
 * @param[in]     font     The font to use for the character.
 * @param[in]     color    The color of the character (WHITE or BLACK).
 *
 * @attention  This function does not write outside the buffer.
 *
 * @return
 *  - DRV_SSD1306_ERR: Error (invalid character or not enough space)
 *  - ch: Success (written character)
 */
char drv_ssd1306_write_char(char ch, drv_ssd1306_font_t font, drv_ssd1306_color_t color,
                            uint8_t char_overflow_flag);

/**
 * @brief  Write a string on the SSD1306 display.
 *
 * @param[in]     str                 The string to write.
 * @param[in]     font                The font to use for the string.
 * @param[in]     color               The color of the string (WHITE or BLACK).
 * @param[in]     char_overflow_flag  Character not fit on the screen, 0: No, 1: overflow at playlist state
 * state, 2: overflow at music player
 *
 * @attention  This function writes each character of the string using the ssd1306_write_char function.
 *
 * @return
 *  - DRV_SSD1306_ERR: Error (character could not be written)
 *  - *str: Success (written character)
 */
char drv_ssd1306_write_string(const char *str, drv_ssd1306_font_t font, drv_ssd1306_color_t color,
                              uint8_t char_overflow_flag);

/**
 * @brief  Draw a line on the SSD1306 display.
 *
 * @param[in]     x1       The x-coordinate of the starting point.
 * @param[in]     y1       The y-coordinate of the starting point.
 * @param[in]     x2       The x-coordinate of the ending point.
 * @param[in]     y2       The y-coordinate of the ending point.
 * @param[in]     color    The color of the line (WHITE or BLACK).
 *
 * @attention  This function uses Bresenham's line algorithm to draw the line.
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                     drv_ssd1306_color_t color);

/**
 * @brief  Draw an arrow on the SSD1306 display.
 *
 * @param[in]     x1                  The x-coordinate of the starting point.
 * @param[in]     y1                  The y-coordinate of the starting point.
 * @param[in]     x2                  The x-coordinate of the ending point.
 * @param[in]     y2                  The y-coordinate of the ending point.
 * @param[in]     arrow_head_size     The size of the arrowhead.
 * @param[in]     color               The color of the arrow (WHITE or BLACK).
 *
 * @attention   This function draws an arrow with a line and an arrowhead using Bresenham's line algorithm.
 *
 * @return			drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_draw_arrow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                           uint8_t arrow_head_size, drv_ssd1306_color_t color);

/**
 * @brief  Draw a rectangle on the SSD1306 display.
 *
 * @param[in]     x1       The x-coordinate of the top-left corner.
 * @param[in]     y1       The y-coordinate of the top-left corner.
 * @param[in]     x2       The x-coordinate of the bottom-right corner.
 * @param[in]     y2       The y-coordinate of the bottom-right corner.
 * @param[in]     color    The color of the rectangle (WHITE or BLACK).
 *
 * @attention   This function draws a rectangle using four lines.
 *
 * @return		  drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                               drv_ssd1306_color_t color);

/**
 * @brief  Fill a rectangle on the SSD1306 display.
 *
 * @param[in]     x1       The x-coordinate of the top-left corner.
 * @param[in]     y1       The y-coordinate of the top-left corner.
 * @param[in]     x2       The x-coordinate of the bottom-right corner.
 * @param[in]     y2       The y-coordinate of the bottom-right corner.
 * @param[in]     color    The color to fill the rectangle (WHITE or BLACK).
 *
 * @attention  This function fills a rectangle by drawing pixels within the specified coordinates.
 *
 * @return		 drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error
 */
drv_ssd1306_error_t drv_ssd1306_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                               drv_ssd1306_color_t color);
/**
 * @brief  Draw a circle on the SSD1306 display.
 *
 * @param[in]     center_x       The x-coordinate of the center of the circle.
 * @param[in]     center_y       The y-coordinate of the center of the circle.
 * @param[in]     radius         The radius of the circle.
 * @param[in]     color          The color of the circle (WHITE or BLACK).
 *
 * @attention  This function uses the midpoint circle algorithm to draw the circle.
 *
 * @return      drv_ssd1306_error_t  	Function status
 *  - DRV_SSD1306_OK:   Success
 *  - DRV_SSD1306_ERR:  Error (coordinates out of bounds)
 */
drv_ssd1306_error_t drv_ssd1306_draw_circle(uint8_t center_x, uint8_t center_y, uint8_t radius,
                                            drv_ssd1306_color_t color);

/**
 * @brief  Fill a circle on the SSD1306 display.
 *
 * @param[in]     center_x       The x-coordinate of the center of the circle.
 * @param[in]     center_y       The y-coordinate of the center of the circle.
 * @param[in]     radius         The radius of the circle.
 * @param[in]     color          The color to fill the circle (WHITE or BLACK).
 *
 * @attention  This function uses the midpoint circle algorithm to fill the circle.
 *
 * @return
 *  - DRV_SSD1306_OK: Success
 *  - DRV_SSD1306_ERR: Error (coordinates out of bounds)
 */
drv_ssd1306_error_t drv_ssd1306_fill_circle(uint8_t center_x, uint8_t center_y, uint8_t radius,
                                            drv_ssd1306_color_t color);

/**
 * @brief  Draw a bitmap on the SSD1306 display.
 *
 * @param[in]     x           The x-coordinate of the top-left corner of the bitmap.
 * @param[in]     y           The y-coordinate of the top-left corner of the bitmap.
 * @param[in]     bitmap      The bitmap data to draw.
 * @param[in]     width       The width of the bitmap.
 * @param[in]     height      The height of the bitmap.
 * @param[in]     color       The color of the bitmap (WHITE or BLACK).
 *
 * @attention  This function draws a bitmap on the display using the provided bitmap data.
 *
 * @return
 *  - DRV_SSD1306_OK: Success
 *  - DRV_SSD1306_ERR: Error (coordinates out of bounds)
 */
drv_ssd1306_error_t drv_ssd1306_draw_bitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t width,
                                            uint8_t height, drv_ssd1306_color_t color);
#endif  // __DRV_SSD1306_H
        /* End of file -------------------------------------------------------- */
