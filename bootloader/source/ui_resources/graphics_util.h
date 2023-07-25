/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include "font.h"

typedef struct _gfx_surface
{
    int height;
    int width;
    int pitch;
    void *buf;
} gfx_surface_t;

/*! @brief Draw a rectangular inside a buffer in RGB565
 * @param pOverlay Describe the attributes of the surface
 * @param x Starting position on the width axis
 * @param y Starting position on the height axis
 * @param w Width of the rectangular
 * @param h Height of the rectangular
 * @param color Color to fill the rectangular in RGB565
 */
int Graphic_DrawRect(gfx_surface_t *pOverlay, const int x, const int y, const int w, const int h, const int color);

/*! @brief Copy a picture inside a surface
 * @param pOverlay Describe the attributes of the surface
 * @param x Starting position on the width axis
 * @param y Starting position on the height axis
 * @param w Width of the picture
 * @param h Height of the picture
 * @param alpha
 * &param *pIcon Pointer to Image stored in memory
 */
int Graphic_DrawPicture(
    gfx_surface_t *pOverlay, const int x, const int y, const int w, const int h, const int alpha, const char *pIcon);

/*! @brief Write text on a surface
 * @param pOverlay Describe the attributes of the surface
 * @param x Starting position on the width axis
 * @param y Starting position on the height axis
 * @param text_color Format RGB565
 * @param bg_color background of the text. Use 0x0 for transparent
 * @param type Font type check the font_t for all the allowed fonts
 * &param *pText Pointer to a NULL terminated string
 */
int Graphic_DrawText(gfx_surface_t *pOverlay,
                     const int x,
                     const int y,
                     const int text_color,
                     const int bg_color,
                     const font_t type,
                     const char *pText);

#endif /* GRAPHICS_UTILS_H */
