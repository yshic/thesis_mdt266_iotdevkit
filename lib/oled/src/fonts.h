/**
 * @file       fonts.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-07-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for fonts
 *
 * @note       None
 * @example    None
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __FONTS_H
#define __FONTS_H

/* Includes ----------------------------------------------------------- */
#include "drv_ssd1306.h"

/* Public defines ----------------------------------------------------- */
#ifdef SSD1306_INCLUDE_FONT_6X8
extern const drv_ssd1306_font_t font_6x8;
#endif
#ifdef SSD1306_INCLUDE_FONT_7X10
extern const drv_ssd1306_font_t font_7x10;
#endif
#ifdef SSD1306_INCLUDE_FONT_11X18
extern const drv_ssd1306_font_t font_11x18;
#endif
#ifdef SSD1306_INCLUDE_FONT_16X26
extern const drv_ssd1306_font_t font_16x26;
#endif
#ifdef SSD1306_INCLUDE_FONT_16X24
extern const drv_ssd1306_font_t font_16x24;
#endif
#ifdef SSD1306_INCLUDE_FONT_16X15
extern const drv_ssd1306_font_t font_16x15;
#endif
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif /* __FONTS_H */

/* End of file -------------------------------------------------------- */
