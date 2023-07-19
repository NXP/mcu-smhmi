/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_SAFE_STRLEN_H_
#define _SLN_SAFE_STRLEN_H_

#include <stddef.h>
#ifndef __REDLIB__
#include <string.h>
#endif /* __REDLIB__ */

/**
 * @brief Determine the length of a fixed-size string.
 *        For __REDLIB__, determine the length manually.
 *        For NOT __REDLIB__, call strnlen from <string.h>.
 *
 * @param ptr Pointer to the data.
 * @param max Maximum length to be counted.
 *
 * @return Number of bytes in the string pointed by 'ptr', but at most 'max'.
 */
__attribute__((weak)) size_t safe_strlen(const char *ptr, size_t max)
{
#ifdef __REDLIB__
    /* REDLIB Has no strnlen call; This serves as a placeholder for developer to implement own function. */
    size_t len;

    for (len = 0; len < max; len++, ptr++)
    {
        if (*ptr == 0)
        {
            break;
        }
    }

    return len;
#else
    return strnlen(ptr, max);
#endif /* __REDLIB__ */
}

#endif /* _SLN_SAFE_STRLEN_H_ */
