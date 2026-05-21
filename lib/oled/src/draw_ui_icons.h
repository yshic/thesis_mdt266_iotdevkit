/**
 * @file       draw_ui_icons.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-07-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for draw_ui_icons
 *
 * @note       This file is for storing and drawing screens ui and bitmaps
 * @example    None
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRAW_UI_ICONS_H
#define __DRAW_UI_ICONS_H

/* Includes ----------------------------------------------------------- */
#include "drv_ssd1306.h"
#include "fonts.h"
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

/**
 * @brief  Draw the starting screen
 *
 * @param[in]   void
 *
 * @attention   This function draws the starting screen by printing the Malkman logo
 *
 * @return      void
 */
void ssd1306_starting_screen(void);

/**
 * @brief  Draw the playlist screen
 *
 * @param[in]   void
 *
 * @attention   None
 *
 * @return      void
 */

/**** Music Player UI ****/
/**
 * @brief  Display the playlist menu
 *
 * This function shows the playlist menu on the display with the current song names.
 *
 * @param[in]     song_id  The ID of the current song
 *
 * @attention  None
 *
 * @return  None
 */
void ssd1306_playlist_menu(uint16_t song_id, int volume);

/**
 * @brief  Display the default music player menu
 *
 * This function shows the default music player menu on the display with the current song ID, playback state,
 * and play mode state.
 *
 * @param[in]     song_id         The ID of the current song
 * @param[in]     playback_state  The current playback state (0 = Pause, 1 = Play)
 * @param[in]     play_mode_state The current play mode state (0 = Once, 1 = Repeat, 2 = Randomize)
 *
 * @attention  None
 *
 * @return  None
 */
void ssd1306_music_player_menu_default(uint16_t song_id, uint8_t playback_state, uint8_t play_mode_state);

/**
 * @brief  Display the music player title
 *
 * This function shows the music player title on the display with the current song ID.
 *
 * @param[in]     song_id  The ID of the current song
 *
 * @attention  None
 *
 * @return  None
 */
void ssd1306_music_player_menu_title(uint16_t song_id, int volume);

/**
 * @brief  Display the play button
 *
 * This function shows the play button on the display.
 *
 * @param         void
 *
 * @attention     None
 *
 * @return        None
 */
void ssd1306_music_player_menu_play(void);

/**
 * @brief  Display the pause button
 *
 * This function shows the pause button on the display.
 *
 * @param      void
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_music_player_menu_pause(void);

/**
 * @brief  Display the once play mode button
 *
 * This function shows the once play mode button on the display.
 *
 * @param      void
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_music_player_menu_once(void);

/**
 * @brief  Display the repeat play mode button
 *
 * This function shows the repeat play mode button on the display.
 *
 * @param      void
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_music_player_menu_repeat(void);

/**
 * @brief  Display the randomize play mode button
 *
 * This function shows the randomize play mode button on the display.
 *
 * @param      void
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_music_player_menu_randomize(void);

/**
 * @brief  Draw the menu buttons based on the playback and play mode states
 *
 * This function draws the appropriate buttons on the display based on the current playback and play mode
 * states.
 *
 * @param[in]     playback_state  The current playback state (0 = Pause, 1 = Play, 99 = Uninitialized)
 * @param[in]     play_mode_state The current play mode state (0 = Once, 1 = Repeat, 2 = Randomize, 99 =
 * Uninitialized)
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_draw_menu_button_fsm(uint8_t playback_state, uint8_t play_mode_state);

/**
 * @brief  Select a menu item
 *
 * This function highlights a menu item by drawing a filled circle and inverting the bitmap colors.
 *
 * @param[in]     x       The x-coordinate of the menu item
 * @param[in]     y       The y-coordinate of the menu item
 * @param[in]     rad     The radius of the filled circle
 * @param[in]     color   The color of the filled circle
 * @param[in]     bitmap  The bitmap of the menu item
 * @param[in]     w       The width of the bitmap
 * @param[in]     h       The height of the bitmap
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_select(uint8_t x, uint8_t y, uint8_t rad, drv_ssd1306_color_t color, const unsigned char* bitmap,
                    uint8_t w, uint8_t h);

/**
 * @brief  Deselect a menu item
 *
 * This function removes the highlight from a menu item by drawing an inverted filled circle and restoring the
 * bitmap colors.
 *
 * @param[in]     x       The x-coordinate of the menu item
 * @param[in]     y       The y-coordinate of the menu item
 * @param[in]     rad     The radius of the filled circle
 * @param[in]     color   The color of the filled circle
 * @param[in]     bitmap  The bitmap of the menu item
 * @param[in]     w       The width of the bitmap
 * @param[in]     h       The height of the bitmap
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_deselect(uint8_t x, uint8_t y, uint8_t rad, drv_ssd1306_color_t color,
                      const unsigned char* bitmap, uint8_t w, uint8_t h);

/**
 * @brief  Deselect menu items based on the playback and play mode states
 *
 * This function removes the highlight from the appropriate menu items based on the current playback and play
 * mode states.
 *
 * @param[in]     playback_state  The current playback state (0 = Pause, 1 = Play, 99 = Uninitialized)
 * @param[in]     play_mode_state The current play mode state (0 = Once, 1 = Repeat, 2 = Randomize, 99 =
 * Uninitialized)
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_deselect_fsm(uint8_t playback_state, uint8_t play_mode_state);

/**
 * @brief  Transition an icon on the display
 *
 * This function transitions an icon on the display by filling a rectangle with an inverted color and drawing
 * the bitmap.
 *
 * @param[in]     x       The x-coordinate of the icon
 * @param[in]     y       The y-coordinate of the icon
 * @param[in]     bitmap  The bitmap of the icon
 * @param[in]     w       The width of the bitmap
 * @param[in]     h       The height of the bitmap
 * @param[in]     color   The color of the icon
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_icon_transition(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h,
                             drv_ssd1306_color_t color);

/**
 * @brief  Scroll a string on the display
 *
 * This function scrolls a string on the display while the scroll flag is active.
 *
 * @param[in]     str           The string to scroll
 * @param[in]     x             The x-coordinate of the string
 * @param[in]     y             The y-coordinate of the string
 * @param[in]     font          The font of the string
 * @param[in]     color         The color of the string
 * @param[in]     screen_state  The current screen state
 *
 * @attention  None
 *
 * @return     None
 */
void ssd1306_scroll_string(const char* str, uint8_t x, uint8_t y, drv_ssd1306_font_t Font,
                           drv_ssd1306_color_t color, uint8_t screen_state);

/**
 * @brief  Get the scroll flag status
 *
 * This function returns the current status of the scroll flag.
 *
 * @param      void
 *
 * @attention  None
 *
 * @return     The current status of the scroll flag
 */
bool ssd1306_get_scroll_flag(void);

/**
 * @brief  Set the scroll flag status
 *
 * This function sets the status of the scroll flag.
 *
 * @param[in]   status  The new status of the scroll flag
 *
 * @attention   None
 *
 * @return      None
 */
void ssd1306_set_scroll_flag(bool status);
#endif /* __DRAW_UI_ICONS_H */

/* End of file -------------------------------------------------------- */
