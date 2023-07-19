/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <ui_resources/fonts/font.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "fsl_common.h"
#include "graphics_util.h"

int Graphic_DrawRect(gfx_surface_t *pOverlay, const int x, const int y, const int w, const int h, const int color)
{
    int error = 0;

    uint32_t startOdd       = 0;
    uint32_t endOdd         = 0;
    uint32_t m              = MIN((x + w), pOverlay->width);
    uint32_t n              = MIN((y + h), pOverlay->height);
    uint32_t rgb565Width    = pOverlay->pitch / sizeof(uint16_t);
    uint16_t *pCanvasBuffer = (uint16_t *)pOverlay->buf;
    uint32_t color32        = ((color & 0xFFFF) << 16) | (color & 0xFFFF);
    uint16_t color16        = (color & 0xFFFF);

    startOdd = x & 1;
    endOdd   = m & 1;

    for (int32_t i = y; i < n; i++)
    {
        int32_t j = x;
        if (startOdd)
        {
            *(pCanvasBuffer + i * rgb565Width + j++) = color16;
        }

        for (; j < m; j += 2)
        {
            *(uint32_t *)(pCanvasBuffer + i * rgb565Width + j) = color32;
        }

        if (endOdd)
        {
            *(pCanvasBuffer + i * rgb565Width + j - 1) = color16;
        }
    }

    return error;
}

int Graphic_DrawPicture(
    gfx_surface_t *pOverlay, const int x, const int y, const int w, const int h, const int alpha, const char *pIcon)
{
    int error = 0;

    uint32_t h_end              = y + h;
    uint32_t w_end              = x + w;
    uint16_t *pCanvasBuffer     = (uint16_t *)pOverlay->buf;
    uint32_t rgb565Width        = pOverlay->pitch / sizeof(uint16_t);
    const uint16_t *pIconRgb565 = (uint16_t *)pIcon;

    if ((alpha & 0xFFFF) != 0xFFFF)
    {
        for (int i = y; i < h_end; i++)
        {
            for (int j = x; j < w_end; j++)
            {
                if (*pIconRgb565 <= alpha)
                {
                    *(pCanvasBuffer + i * rgb565Width + j) = *pIconRgb565++;
                }
                else
                {
                    pIconRgb565++;
                }
            }
        }
    }
    else /* in most cases, we can do optimization by 2 pixels per operation */
    {
        uint32_t start_odd = x & 1;
        uint32_t end_odd   = w_end & 1;

        for (int i = y; i < h_end; i++)
        {
            int j = x;
            if (start_odd)
            {
                *(pCanvasBuffer + i * rgb565Width + j++) = *pIconRgb565++;
            }

            for (; j < w_end; j += 2)
            {
                *(uint32_t *)(pCanvasBuffer + i * rgb565Width + j) = *(uint32_t *)pIconRgb565;
                pIconRgb565 += 2;
            }

            if (end_odd)
            {
                *(pCanvasBuffer + i * rgb565Width + j - 1) = *pIconRgb565++;
            }
        }
    }

    return error;
}

int Graphic_DrawText(gfx_surface_t *pOverlay,
                     const int x,
                     const int y,
                     const int text_color,
                     const int bg_color,
                     const font_t type,
                     const char *pText)
{
    int error = 0;

    uint16_t *pCanvasBuffer = (uint16_t *)pOverlay->buf;
    int rgb565Width         = pOverlay->pitch / sizeof(uint16_t);
    put_string_utf8(x, y, (char *)pText, (unsigned short)(text_color & 0xFFFF), (unsigned short)(bg_color & 0xFFFF),
                    (font_t)type, pCanvasBuffer, rgb565Width);

    return error;
}
