// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

#ifndef __CUSTOM_H_
#define __CUSTOM_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*********************
 *      INCLUDES
 *********************/
#include "gui_guider.h"

/*********************
 *      DEFINES
 *********************/
#define NO_DELAY              0
#define LVGL_MULTITHREAD_LOCK 1

/* VIT ASR does not support French currently.
 * When VIT is used we must set this definition to 0. */
#define FRENCH_LANG_SUPPORTED 1

/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
    kScreen_Main = 0,
    kScreen_Home,
    kScreen_Brewing,
    kScreen_Finished,
    kScreen_Standby,
    kScreen_Num
} screen_t;

typedef enum
{
    kLanguage_EN = 0,
    kLanguage_CN,
    kLanguage_DE,
    kLanguage_FR,
    kLanguage_Ids
} language_t;

typedef enum
{
    kCoffeeType_Invalid = -1,
    kCoffeeType_Americano,
    kCoffeeType_Cappuccino,
    kCoffeeType_Espresso,
    kCoffeeType_Latte,
    kCoffeeType_NumTypes
} coffee_type_t;

typedef enum
{
    kCoffeeSize_Invalid = -1,
    kCoffeeSize_Small,
    kCoffeeSize_Medium,
    kCoffeeSize_Large,
    kCoffeeSize_NumSizes
} coffee_size_t;

typedef enum
{
    kCoffeeStrength_Invalid = -1,
    kCoffeeStrength_Soft,
    kCoffeeStrength_Medium,
    kCoffeeStrength_Strong,
    kCoffeeStrength_NumStrengths
} coffee_strength_t;

#define TABLE_DEBUGINFO_BRIGHTNESS_LOW  50
#define TABLE_DEBUGINFO_BRIGHTNESS_HIGH 180

/**********************
 *
 **********************/
LV_IMG_DECLARE(finished_animimg_cafelatteCafe_Latte_Selected_Black);
LV_IMG_DECLARE(finished_animimg_espressoEspresso_Selected_Black);
LV_IMG_DECLARE(finished_animimg_cappccinoCappuccinno_selected_Black);
LV_IMG_DECLARE(finished_animimg_americanoAmericano_Selected_Black);

#include "lvgl/src/extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(brewing_animimg_brewingf01)
LV_IMG_DECLARE(brewing_animimg_brewingf03)
LV_IMG_DECLARE(brewing_animimg_brewingf05)
LV_IMG_DECLARE(brewing_animimg_brewingf07)
LV_IMG_DECLARE(brewing_animimg_brewingf09)
LV_IMG_DECLARE(brewing_animimg_brewingf11)
LV_IMG_DECLARE(brewing_animimg_brewingf13)
LV_IMG_DECLARE(brewing_animimg_brewingf15)
LV_IMG_DECLARE(brewing_animimg_brewingf17)
LV_IMG_DECLARE(brewing_animimg_brewingf19)
LV_IMG_DECLARE(brewing_animimg_brewingf21)
LV_IMG_DECLARE(brewing_animimg_brewingf23)
LV_IMG_DECLARE(brewing_animimg_brewingf25)
LV_IMG_DECLARE(brewing_animimg_brewingf27)
LV_IMG_DECLARE(brewing_animimg_brewingf29)
LV_IMG_DECLARE(brewing_animimg_brewingf31)

LV_IMG_DECLARE(screen_animimg_screensaver01)
LV_IMG_DECLARE(screen_animimg_screensaver02)
LV_IMG_DECLARE(screen_animimg_screensaver03)
LV_IMG_DECLARE(screen_animimg_screensaver04)
LV_IMG_DECLARE(screen_animimg_screensaver05)
LV_IMG_DECLARE(screen_animimg_screensaver06)
LV_IMG_DECLARE(screen_animimg_screensaver07)
LV_IMG_DECLARE(screen_animimg_screensaver08)

#ifdef RT_PLATFORM
/**
 * @brief Signal the UI that a valid interaction took place
 */
void UI_WidgetInteraction_Callback(void);

/**
 * @brief enter screen callback
 * @return 1 if the enter screen operation was a success, 0 otherwise
 */
uint8_t UI_EnterScreen_Callback(screen_t screenId);

/**
 * @brief Async call to the UI to advertise set language.
 */
void UI_SetLanguage_Callback(language_t language);

void UI_Finished_Callback();

void UI_WidgetInteraction_Callback();

void DeregisterCoffeeSelection();

bool FaceRecDebugOptionTrigger();

#else
#define LVGL_LOG printf
#define UI_WidgetInteraction_Callback()
#define UI_EnterScreen_Callback(scr) (1)
#define UI_SetLanguage_Callback(language)
void UI_Finished_Callback();
#define DeregisterCoffeeSelection()
#define FaceRecDebugOptionTrigger() true
void WakeUp(int source);
#endif /*RT_PLATFORM*/

coffee_size_t get_coffee_size(void);
coffee_type_t get_coffee_type(void);
coffee_strength_t get_coffee_strength(void);
const char *get_screen_name(screen_t scr);
language_t get_language(void);
screen_t get_current_screen(void);

void gui_set_home();
void gui_set_brewing();
void gui_set_finished();
void gui_set_standby();

void gui_set_home_coffee_type(coffee_type_t type);
void gui_set_home_coffee_size(coffee_size_t size);
void gui_set_home_coffee_strength(coffee_strength_t strength);

void gui_home_set_language(language_t lang);
void gui_home_language_changed_cb(void);
void gui_brewing_anim_ready_cb(struct _lv_anim_t *anim);
void gui_finished_anim_ready_cb(struct _lv_anim_t *anim);
void gui_standby_language_changed_cb(void);
void gui_main_anim_ready_cb(struct _lv_anim_t *anim);

void gui_enable_deregister_button(bool enable);
void gui_set_virtual_face(lv_img_dsc_t *pImage);
void gui_set_algo_debuginfo(uint8_t isDetFace,
                            uint8_t isSmallFace,
                            uint8_t isBlurry,
                            uint8_t isSideface,
                            uint8_t brightness,
                            int recognized,
                            int id);

void custom_init(lv_ui *ui);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* EVENT_CB_H_ */
