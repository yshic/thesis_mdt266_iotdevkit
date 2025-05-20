/**
 * @file       drv_ssd1306.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-07-31
 * @author     Tuan Nguyen
 *
 * @brief      Driver file for ssd1306
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "drv_ssd1306.h"
#include "config.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static uint8_t       ssd1306_buffer[SSD1306_BUFFER_SIZE]; // Screen buffer
static drv_ssd1306_t ssd1306;                             // Screen object

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
drv_ssd1306_error_t drv_ssd1306_init(void)
{
  if (!bspI2CExist(SSD1306_I2C_ADDR))
  {
    return DRV_SSD1306_BUSY;
  }
  // Wait for the screen to boot
  DELAY(100);

  // Init OLED
  drv_ssd1306_set_display(DRV_SSD1306_DISPLAY_OFF); // display off
  drv_ssd1306_write_command(0x20);                  // Set Memory Addressing Mode
  drv_ssd1306_write_command(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
  drv_ssd1306_write_command(0xB0); // Set Page Start Address for Page Addressing Mode,0-7
  drv_ssd1306_write_command(0xC8); // Set COM Output Scan Direction
  drv_ssd1306_write_command(0x00); //---set low column address
  drv_ssd1306_write_command(0x10); //---set high column address
  drv_ssd1306_write_command(0x40); //--set start line address
  drv_ssd1306_set_contrast(0xFF);
  drv_ssd1306_write_command(0xA1); //--set segment re-map 0 to 127
  drv_ssd1306_write_command(0xA6); //--set normal color
  drv_ssd1306_write_command(0xA8); //--set multiplex ratio(1 to 64) - CHECK
  drv_ssd1306_write_command(0x3F); //
  drv_ssd1306_write_command(0xA4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content

  drv_ssd1306_write_command(0xD3); //-set display offset - CHECK
  drv_ssd1306_write_command(0x00); //-not offset

  drv_ssd1306_write_command(0xD5); //--set display clock divide ratio/oscillator frequency
  drv_ssd1306_write_command(0xF0); //--set divide ratio

  drv_ssd1306_write_command(0xD9); //--set pre-charge period
  drv_ssd1306_write_command(0x22); //

  drv_ssd1306_write_command(0xDA); //--set com pins hardware configuration - CHECK
  drv_ssd1306_write_command(0x12);
  drv_ssd1306_write_command(0xDB);                 //--set vcomh
  drv_ssd1306_write_command(0x20);                 // 0x20,0.77xVcc
  drv_ssd1306_write_command(0x8D);                 //--set DC-DC enable
  drv_ssd1306_write_command(0x14);                 //
  drv_ssd1306_set_display(DRV_SSD1306_DISPLAY_ON); //--turn on SSD1306 panel

  // Clear screen
  drv_ssd1306_fill(DRV_SSD1306_COLOR_BLACK);

  // Flush buffer to screen
  drv_ssd1306_update_screen();

  // Set default values for screen object
  ssd1306.current_x = 0;
  ssd1306.current_y = 0;

  ssd1306.initialized = 1;

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_write_command(uint8_t byte)
{
  bool ret = false;
  ret      = bspI2CWriteByte(SSD1306_I2C_ADDR, 0x00, byte);
  return (ret == true) ? DRV_SSD1306_OK : DRV_SSD1306_ERR;
}

drv_ssd1306_error_t drv_ssd1306_write_data(uint8_t *buffer, size_t buff_size)
{
  bool ret = false;
  ret      = bspI2CWriteBytes(SSD1306_I2C_ADDR, 0x40, buffer, buff_size);
  return (ret == true) ? DRV_SSD1306_OK : DRV_SSD1306_ERR;
}

drv_ssd1306_error_t drv_ssd1306_fill(drv_ssd1306_color_t color)
{
  uint8_t fill_color = (color == DRV_SSD1306_COLOR_BLACK) ? 0x00 : 0xFF;
  memset(ssd1306_buffer, fill_color, sizeof(ssd1306_buffer));
  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_update_screen(void)
{
  uint8_t pages          = SSD1306_HEIGHT / 8;
  uint8_t x_offset_lower = 0x00 + SSD1306_X_OFFSET_LOWER;
  uint8_t x_offset_upper = 0x10 + SSD1306_X_OFFSET_UPPER;

  for (uint8_t i = 0; i < pages; i++)
  {
    drv_ssd1306_write_command(0xB0 + i); // Set the current RAM page address.
    drv_ssd1306_write_command(x_offset_lower);
    drv_ssd1306_write_command(x_offset_upper);
    drv_ssd1306_write_data(&ssd1306_buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
  }
  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_set_display(drv_ssd1306_display_t status)
{
  uint8_t value;
  if (status)
  {
    value              = 0xAF; // Display on
    ssd1306.display_on = 1;
  }
  else
  {
    value              = 0xAE; // Display off
    ssd1306.display_on = 0;
  }
  drv_ssd1306_write_command(value);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_set_contrast(uint8_t value)
{
  const uint8_t SET_CONTRAST_CONTROL_REGISTER = 0x81;
  drv_ssd1306_write_command(SET_CONTRAST_CONTROL_REGISTER);
  drv_ssd1306_write_command(value);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_set_cursor(uint8_t x, uint8_t y)
{
  ssd1306.current_x = x;
  ssd1306.current_y = y;

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_draw_pixel(uint8_t x, uint8_t y, drv_ssd1306_color_t color)
{
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    // Don't write outside the buffer
    return DRV_SSD1306_ERR;
  }

  // Draw in the right color
  if (color == DRV_SSD1306_COLOR_WHITE)
  {
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  }
  else
  {
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
  return DRV_SSD1306_OK;
}

char drv_ssd1306_write_char(char ch, drv_ssd1306_font_t font, drv_ssd1306_color_t color,
                            uint8_t char_overflow_flag)
{
  uint32_t i, b, j;

  // Check if character is valid
  if (ch < 32 || ch > 126)
    return DRV_SSD1306_ERR;

  // Limit the amount of characters on the screen
  switch (char_overflow_flag)
  {
    case 0:
      // Nothing happen
      break;

    case 1:
      if (128 < (ssd1306.current_x + font.width) || SSD1306_HEIGHT < (ssd1306.current_y + font.height))
      {
        // Not enough space on current line
        return 0;
      }
      break;

    case 2:
      if (90 < (ssd1306.current_x + font.width) || SSD1306_HEIGHT < (ssd1306.current_y + font.height))
      {
        // Not enough space on current line
        return 0;
      }
      break;

    default:
      break;
  }

  // Check remaining space on current line
  if (SSD1306_WIDTH < (ssd1306.current_x + font.width) || SSD1306_HEIGHT < (ssd1306.current_y + font.height))
  {
    // Not enough space on current line
    return DRV_SSD1306_ERR;
  }

  // Use the font to write
  for (i = 0; i < font.height; i++)
  {
    b = font.data[(ch - 32) * font.height + i];
    for (j = 0; j < font.width; j++)
    {
      if ((b << j) & 0x8000)
      {
        drv_ssd1306_draw_pixel(ssd1306.current_x + j, (ssd1306.current_y + i), (drv_ssd1306_color_t) color);
      }
      else
      {
        drv_ssd1306_draw_pixel(ssd1306.current_x + j, (ssd1306.current_y + i), (drv_ssd1306_color_t) !color);
      }
    }
  }

  // The current space is now taken
  ssd1306.current_x += font.char_width ? font.char_width[ch - 32] : font.width;

  // Return written char for validation
  return ch;
}

char drv_ssd1306_write_string(const char *str, drv_ssd1306_font_t font, drv_ssd1306_color_t color,
                              uint8_t char_overflow_flag)
{
  while (*str)
  {
    if (drv_ssd1306_write_char(*str, font, color, char_overflow_flag) != *str)
    {
      // Char could not be written
      return DRV_SSD1306_ERR;
    }
    str++;
  }

  // Success
  return *str;
}

drv_ssd1306_error_t drv_ssd1306_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                     drv_ssd1306_color_t color)
{
  int32_t delta_x = abs(x2 - x1);
  int32_t delta_y = abs(y2 - y1);
  int32_t sign_x  = (x1 < x2) ? 1 : -1;
  int32_t sign_y  = (y1 < y2) ? 1 : -1;
  int32_t error   = delta_x - delta_y;
  int32_t error2;

  drv_ssd1306_draw_pixel(x2, y2, color);

  while ((x1 != x2) || (y1 != y2))
  {
    drv_ssd1306_draw_pixel(x1, y1, color);
    error2 = error * 2;
    if (error2 > -delta_y)
    {
      error -= delta_y;
      x1 += sign_x;
    }

    if (error2 < delta_x)
    {
      error += delta_x;
      y1 += sign_y;
    }
  }

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_draw_arrow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                           uint8_t arrow_head_size, drv_ssd1306_color_t color)
{
  // Draw the main line of the arrow
  drv_ssd1306_line(x1, y1, x2, y2, color);

  // Calculate the angle of the arrow
  float angle = atan2(y2 - y1, x2 - x1);

  // Calculate the coordinates of the arrowhead
  uint8_t arrow_x1 = x2 - arrow_head_size * cos(angle + M_PI / 6);
  uint8_t arrow_y1 = y2 - arrow_head_size * sin(angle + M_PI / 6);
  uint8_t arrow_x2 = x2 - arrow_head_size * cos(angle - M_PI / 6);
  uint8_t arrow_y2 = y2 - arrow_head_size * sin(angle - M_PI / 6);

  // Draw the arrowhead
  drv_ssd1306_line(x2, y2, arrow_x1, arrow_y1, color);
  drv_ssd1306_line(x2, y2, arrow_x2, arrow_y2, color);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                               drv_ssd1306_color_t color)
{
  // Draw the top side
  drv_ssd1306_line(x1, y1, x2, y1, color);
  // Draw the right side
  drv_ssd1306_line(x2, y1, x2, y2, color);
  // Draw the bottom side
  drv_ssd1306_line(x2, y2, x1, y2, color);
  // Draw the left side
  drv_ssd1306_line(x1, y2, x1, y1, color);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                               drv_ssd1306_color_t color)
{
  uint8_t x_start = (x1 <= x2) ? x1 : x2;
  uint8_t x_end   = (x1 <= x2) ? x2 : x1;
  uint8_t y_start = (y1 <= y2) ? y1 : y2;
  uint8_t y_end   = (y1 <= y2) ? y2 : y1;

  for (uint8_t y = y_start; (y <= y_end) && (y < SSD1306_HEIGHT); y++)
  {
    for (uint8_t x = x_start; (x <= x_end) && (x < SSD1306_WIDTH); x++)
    {
      drv_ssd1306_draw_pixel(x, y, color);
    }
  }
  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_draw_circle(uint8_t center_x, uint8_t center_y, uint8_t radius,
                                            drv_ssd1306_color_t color)
{
  int32_t x   = -radius;        // Initialize x to the negative radius
  int32_t y   = 0;              // Initialize y to 0
  int32_t err = 2 - 2 * radius; // Initialize the error term
  int32_t e2;

  // Check if the center coordinates are within the display bounds
  if (center_x >= SSD1306_WIDTH || center_y >= SSD1306_HEIGHT)
  {
    return DRV_SSD1306_ERR;
  }

  do
  {
    // Draw the four symmetric points of the circle
    drv_ssd1306_draw_pixel(center_x - x, center_y + y, color);
    drv_ssd1306_draw_pixel(center_x + x, center_y + y, color);
    drv_ssd1306_draw_pixel(center_x + x, center_y - y, color);
    drv_ssd1306_draw_pixel(center_x - x, center_y - y, color);
    e2 = err;

    // Adjust the error term and coordinates
    if (e2 <= y)
    {
      y++;
      err += y * 2 + 1;
      if (-x == y && e2 <= x)
      {
        e2 = 0;
      }
    }

    if (e2 > x)
    {
      x++;
      err += x * 2 + 1;
    }
  } while (x <= 0);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_fill_circle(uint8_t center_x, uint8_t center_y, uint8_t radius,
                                            drv_ssd1306_color_t color)
{
  int32_t x   = -radius;        // Initialize x to the negative radius
  int32_t y   = 0;              // Initialize y to 0
  int32_t err = 2 - 2 * radius; // Initialize the error term
  int32_t e2;

  // Check if the center coordinates are within the display bounds
  if (center_x >= SSD1306_WIDTH || center_y >= SSD1306_HEIGHT)
  {
    return DRV_SSD1306_ERR;
  }

  do
  {
    // Fill the vertical line for the current x and y
    for (uint8_t par_y = center_y + y; par_y >= center_y - y; par_y--)
    {
      for (uint8_t par_x = center_x - x; par_x >= center_x + x; par_x--)
      {
        drv_ssd1306_draw_pixel(par_x, par_y, color);
      }
    }

    e2 = err;

    // Adjust the error term and coordinates
    if (e2 <= y)
    {
      y++;
      err += y * 2 + 1;
      if (-x == y && e2 <= x)
      {
        e2 = 0;
      }
    }

    if (e2 > x)
    {
      x++;
      err += x * 2 + 1;
    }
  } while (x <= 0);

  return DRV_SSD1306_OK;
}

drv_ssd1306_error_t drv_ssd1306_draw_bitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t width,
                                            uint8_t height, drv_ssd1306_color_t color)
{
  int16_t byte_width = (width + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte       = 0;

  // Check if the coordinates are within the display bounds
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    return DRV_SSD1306_ERR;
  }

  // Loop through each row of the bitmap
  for (uint8_t j = 0; j < height; j++, y++)
  {
    // Loop through each column of the bitmap
    for (uint8_t i = 0; i < width; i++)
    {
      // Load a new byte every 8 pixels
      if (i & 7)
      {
        byte <<= 1; // Shift the byte left by 1 bit
      }
      else
      {
        // Load the next byte from the bitmap data
        byte = (*(const unsigned char *) (&bitmap[j * byte_width + i / 8]));
      }

      // Draw the pixel if the most significant bit is set
      if (byte & 0x80)
      {
        drv_ssd1306_draw_pixel(x + i, y, color);
      }
    }
  }
  return DRV_SSD1306_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
