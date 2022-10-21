// SPDX-License-Identifier: MIT
// Copyright 2022 NXP

/*********************
 *      INCLUDES
 *********************/
#include "custom.h"
#include "lvgl/lvgl.h"
#include <stdio.h>

#include "guider_customer_fonts.h"

/*********************
 *      DEFINES
 *********************/

#ifdef RT_PLATFORM
#include "FreeRTOS.h"
#include "semphr.h"
#define LVGL_LOG(...)
#endif /*RT_PLATFORM*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
#ifdef RT_PLATFORM
/* LVGL should be single thread. Get this mutex for multithread access */
static SemaphoreHandle_t s_LVGLMutex;
#endif /*RT_PLATFORM*/

static coffee_type_t s_CoffeeType         = kCoffeeType_NumTypes;
static coffee_size_t s_CoffeeSize         = kCoffeeSize_NumSizes;
static coffee_strength_t s_CoffeeStrength = kCoffeeStrength_NumStrengths;
static language_t s_Language              = kLanguage_EN;

static const lv_img_dsc_t *s_FinishedAnimImagAmericano[] = {&finished_animimg_americanoAmericano_Selected_Black};

static const lv_img_dsc_t *s_FinishedAnimImagCappuccino[] = {&finished_animimg_cappccinoCappuccinno_selected_Black};

static const lv_img_dsc_t *s_FinishedAnimImagEspresso[] = {&finished_animimg_espressoEspresso_Selected_Black};

static const lv_img_dsc_t *s_FinishedAnimImagCafelatte[] = {&finished_animimg_cafelatteCafe_Latte_Selected_Black};

static char *s_ScreenName[kScreen_Num + 1] = {"Main", "Home", "Brewing", "Finished", "Standby", "Invalid"};

static const char *s_CoffeeTypeStr[kLanguage_Ids][kCoffeeType_NumTypes] = {
    {"Americano", "Cappuccino", "Espresso", "Caffe Latte"},
    {"美式咖啡", "卡布奇诺", "意式咖啡", "拿铁"},
    {"Americano", "Cappuccino", "Espresso", "Café Latte"},
    {"Café Americano", "Cappuccino", "Espresso", "Café Latté"},
};

static const char *s_CoffeeSizeStr[kLanguage_Ids][kCoffeeSize_NumSizes] = {
    {
        "Small",
        "Medium",
        "Large",
    },
    {
        "小杯",
        "中杯",
        "大杯",
    },
    {
        "Klein",
        "Mittel",
        "Groß",
    },
    {
        "Court",
        "Moyen",
        "Long",
    },
};

static const char *s_CoffeeStrengthStr[kLanguage_Ids][kCoffeeStrength_NumStrengths] = {
    {
        "Soft",
        "Mild",
        "Strong",
    },
    {
        "柔润",
        "温和",
        "浓烈",
    },
    {
        "Leicht",
        "Mild",
        "Stark",
    },
    {
        "Léger",
        "Normal",
        "Serré",
    },
};

static const char *s_CoffeeAttrStr[kLanguage_Ids][3] = {
    {
        "Type:",
        "Size:",
        "Strength:",
    },
    {
        "类型",
        "杯型:",
        "浓度:",
    },
    {
        "Type:",
        "Größe:",
        "Stärke:",
    },
    {
        "Type:",
        "Taille:",
        "Intensité:",
    },
};

static const char *s_StartStr[kLanguage_Ids][1] = {
    {
        "START",
    },
    {
        "开始",
    },
    {
        "STARTEN",
    },
    {
        "COMMENCER",
    },
};

static const char *s_StandbyHelpStr[kLanguage_Ids][4] = {
    {"Say \"Hey, NXP\"", "or", "Touch the screen", "to select your coffee"},
    {"请说：“你好，恩智浦”", "或者", "触摸此屏幕", "来选择您的咖啡"},
    {"Sagen \"Hallo, NXP\"", "oder", "Berühre den Bildschirm", "Wähle deinen Kaffee"},
    {"Dites \"Salut NXP\"", "ou", "Appuyez sur l’écran", "Pour sélectionner votre café"},
};

static const lv_img_dsc_t *s_BrewingAnimImgs[] = {
    &brewing_animimg_brewingf01, &brewing_animimg_brewingf03, &brewing_animimg_brewingf05, &brewing_animimg_brewingf07,
    &brewing_animimg_brewingf09, &brewing_animimg_brewingf11, &brewing_animimg_brewingf13, &brewing_animimg_brewingf15,
    &brewing_animimg_brewingf17, &brewing_animimg_brewingf19, &brewing_animimg_brewingf21, &brewing_animimg_brewingf23,
    &brewing_animimg_brewingf25, &brewing_animimg_brewingf27, &brewing_animimg_brewingf29, &brewing_animimg_brewingf31};

static const lv_img_dsc_t *s_StandbyAnimImgs[] = {&screen_animimg_screensaver01, &screen_animimg_screensaver02,
                                                  &screen_animimg_screensaver03, &screen_animimg_screensaver04,
                                                  &screen_animimg_screensaver05, &screen_animimg_screensaver06,
                                                  &screen_animimg_screensaver07, &screen_animimg_screensaver08};

#ifdef RT_PLATFORM
void _takeLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreTake(s_LVGLMutex, portMAX_DELAY);
    }
}

void _giveLVGLMutex()
{
    if (s_LVGLMutex)
    {
        xSemaphoreGive(s_LVGLMutex);
    }
}
#else
void WakeUp(int source)
{
    gui_set_home();
}

void UI_Finished_Callback()
{
    gui_set_standby();
}
#endif /*RT_PLATFORM*/

/**
 * @brief Set the type of coffee to brew
 *
 * @param type the type of coffee to brew
 */
static void set_coffee_type(coffee_type_t type)
{
    if (kCoffeeType_Invalid < type && type < kCoffeeType_NumTypes)
    {
        s_CoffeeType = type;
    }
}

/**
 * @brief Set the size of coffee to brew
 *
 * @param size the size of coffee to brew
 */
static void set_coffee_size(coffee_size_t size)
{
    if (kCoffeeSize_Invalid < size && size < kCoffeeSize_NumSizes)
    {
        s_CoffeeSize = size;
    }
}

/**
 * @brief Set the strength of coffee to brew
 *
 * @param strength the current strength of coffee to brew
 */
static void set_coffee_strength(coffee_strength_t strength)
{
    if (kCoffeeStrength_Invalid < strength && strength < kCoffeeStrength_NumStrengths)
    {
        s_CoffeeStrength = strength;
    }
}

/**
 * @brief Set the language
 *
 * @return language_t of the current language
 */
static void set_language(language_t lang)
{
    if ((lang >= kLanguage_EN) && (lang < kLanguage_Ids))
    {
        s_Language = lang;
    }
}

/**
 * @brief Get the currently selected coffee size
 *
 * @return coffee_size_t the current size of coffee to brew
 */
coffee_size_t get_coffee_size(void)
{
    return s_CoffeeSize;
}

/**
 * @brief Get the currently selected coffee type
 *
 * @return coffee_type_t the currently type of coffee to brew
 */
coffee_type_t get_coffee_type(void)
{
    return s_CoffeeType;
}

/**
 * @brief Get the currently selected coffee strength
 *
 * @return coffee_strength_t the strength of coffee to brew
 */
coffee_strength_t get_coffee_strength(void)
{
    return s_CoffeeStrength;
}

/**
 * @brief Get the language
 *
 * @return language_t of the current language
 */
language_t get_language(void)
{
    return s_Language;
}

/**
 * @brief get screen name
 *
 */
const char *get_screen_name(screen_t scr)
{
    return s_ScreenName[scr];
}

/**
 * @brief get current screen
 *
 */
screen_t get_current_screen(void)
{
    lv_obj_t *pCurrentScreen = lv_scr_act();
    screen_t currentScreenId = kScreen_Num;

    if (pCurrentScreen == guider_ui.main)
    {
        currentScreenId = kScreen_Main;
    }
    else if (pCurrentScreen == guider_ui.home)
    {
        currentScreenId = kScreen_Home;
    }
    else if (pCurrentScreen == guider_ui.brewing)
    {
        currentScreenId = kScreen_Brewing;
    }
    else if (pCurrentScreen == guider_ui.finished)
    {
        currentScreenId = kScreen_Finished;
    }
    else if (pCurrentScreen == guider_ui.standby)
    {
        currentScreenId = kScreen_Standby;
    }

    return currentScreenId;
}

/**
 * @brief Animate the coffee fill progress bar by setting its value
 *
 * @param bar the bar object to set the value of
 * @param v the value to set the bar to
 */
static void _set_bar_value(void *bar, int32_t v)
{
    lv_bar_set_value(bar, v, LV_ANIM_OFF);
}

static void _set_label_font(lv_obj_t *label, language_t lang)
{
    if ((lang == kLanguage_CN))
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_32};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_32};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _set_label_font_20(lv_obj_t *label, language_t lang)
{
    if (lang == kLanguage_CN)
    {
        lv_style_value_t v = {.ptr = &lv_font_simsun_20};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_style_value_t v = {.ptr = &lv_font_montserratMedium_20};
        lv_obj_set_local_style_prop(label, LV_STYLE_TEXT_FONT, v, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

static void _home_img_nxpFaceRec_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            bool isEnabled = FaceRecDebugOptionTrigger();
            if (isEnabled)
            {
                lv_obj_clear_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
            }
        }
        break;
        default:
            break;
    }
}

static void standby_wake_up_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_CLICKED:
        {
            WakeUp(2);
        }
        break;
        default:
            break;
    }
}

/**
 * @brief Check if all the parameters have been set
 *
 * @return true
 * @return false
 */
static bool gui_check_coffee_selection()
{
    bool validCoffeeSelection = true;
    if ((s_CoffeeType == kCoffeeType_Invalid) || (s_CoffeeType == kCoffeeType_NumTypes))
    {
        validCoffeeSelection = false;
    }
    else if ((s_CoffeeSize == kCoffeeSize_Invalid) || (s_CoffeeSize == kCoffeeSize_NumSizes))
    {
        validCoffeeSelection = false;
    }
    else if ((s_CoffeeStrength == kCoffeeStrength_Invalid) || (s_CoffeeStrength == kCoffeeStrength_NumStrengths))
    {
        validCoffeeSelection = false;
    }

    return validCoffeeSelection;
}

/**
 * @brief Enable the start button allowing press events
 *
 * @param enable if enable = true check if the coffee has been properly configured and enable it.
 * if enable = false. Disable press event and make it checked
 */
static void gui_enable_start_button(bool enable)
{
    if (enable == true)
    {
        if (gui_check_coffee_selection())
        {
            lv_obj_clear_state(guider_ui.home_imgbtn_start, LV_STATE_CHECKED);
            lv_obj_add_flag(guider_ui.home_imgbtn_start, LV_OBJ_FLAG_CLICKABLE);
        }
    }
    else
    {
        lv_obj_add_state(guider_ui.home_imgbtn_start, LV_STATE_CHECKED);
        lv_obj_clear_flag(guider_ui.home_imgbtn_start, LV_OBJ_FLAG_CLICKABLE);
    }
}

void gui_set_coffee_selection(lv_ui *ui, screen_t scr)
{
    if (scr == kScreen_Brewing)
    {
        if ((s_CoffeeType >= kCoffeeType_Americano) && (s_CoffeeType < kCoffeeType_NumTypes))
        {
            _set_label_font(guider_ui.brewing_label_coffee_type, s_Language);
            lv_label_set_text(guider_ui.brewing_label_coffee_type, s_CoffeeTypeStr[s_Language][s_CoffeeType]);
        }

        if ((s_CoffeeSize >= kCoffeeSize_Small) && (s_CoffeeSize < kCoffeeSize_NumSizes))
        {
            _set_label_font(guider_ui.brewing_label_coffee_size, s_Language);
            lv_label_set_text(guider_ui.brewing_label_coffee_size, s_CoffeeSizeStr[s_Language][s_CoffeeSize]);
        }

        if ((s_CoffeeStrength >= kCoffeeStrength_Soft) && (s_CoffeeStrength < kCoffeeStrength_NumStrengths))
        {
            _set_label_font(guider_ui.brewing_label_coffee_strength, s_Language);
            lv_label_set_text(guider_ui.brewing_label_coffee_strength,
                              s_CoffeeStrengthStr[s_Language][s_CoffeeStrength]);
        }
    }
    else if (scr == kScreen_Finished)
    {
        if ((s_CoffeeType >= kCoffeeType_Americano) && (s_CoffeeType < kCoffeeType_NumTypes))
        {
            _set_label_font(guider_ui.finished_label_coffee_type, s_Language);
            lv_label_set_text(guider_ui.finished_label_coffee_type, s_CoffeeTypeStr[s_Language][s_CoffeeType]);
        }

        if ((s_CoffeeSize >= kCoffeeSize_Small) && (s_CoffeeSize < kCoffeeSize_NumSizes))
        {
            _set_label_font(guider_ui.finished_label_coffee_size, s_Language);
            lv_label_set_text(guider_ui.finished_label_coffee_size, s_CoffeeSizeStr[s_Language][s_CoffeeSize]);
        }

        if ((s_CoffeeStrength >= kCoffeeStrength_Soft) && (s_CoffeeStrength < kCoffeeStrength_NumStrengths))
        {
            _set_label_font(guider_ui.finished_label_coffee_strength, s_Language);
            lv_label_set_text(guider_ui.finished_label_coffee_strength,
                              s_CoffeeStrengthStr[s_Language][s_CoffeeStrength]);
        }
    }
}

void gui_set_brewing_animation(lv_ui *ui)
{
    uint8_t animimg_num = sizeof(s_BrewingAnimImgs) / sizeof(lv_img_dsc_t *);
    LVGL_LOG("+gui_set_brewing_animation:%d\r\n", animimg_num);
    lv_animimg_set_src(ui->brewing_animimg_brewing, (lv_img_dsc_t **)s_BrewingAnimImgs, animimg_num);
    lv_animimg_set_duration(ui->brewing_animimg_brewing, animimg_num * 150);
    lv_animimg_t *brewing_animimg_brewing_ext = (lv_animimg_t *)ui->brewing_animimg_brewing;
    lv_anim_set_ready_cb(&brewing_animimg_brewing_ext->anim, gui_brewing_anim_ready_cb);
    lv_animimg_set_repeat_count(ui->brewing_animimg_brewing, 1);

    lv_animimg_set_exec_cb(ui->brewing_animimg_brewing, _set_bar_value, (void *)ui->brewing_bar_fillStatus);
    lv_animimg_start(ui->brewing_animimg_brewing);
}

void gui_set_home_coffee_type(coffee_type_t type)
{
    if ((type >= kCoffeeType_Americano) && (type <= kCoffeeType_NumTypes))
    {
        lv_obj_t *coffee_type_imgbtns[kCoffeeType_NumTypes];
        coffee_type_imgbtns[kCoffeeType_Americano]  = guider_ui.home_imgbtn_americano;
        coffee_type_imgbtns[kCoffeeType_Cappuccino] = guider_ui.home_imgbtn_cappuccino;
        coffee_type_imgbtns[kCoffeeType_Espresso]   = guider_ui.home_imgbtn_espresso;
        coffee_type_imgbtns[kCoffeeType_Latte]      = guider_ui.home_imgbtn_caffelatte;
        s_CoffeeType                                = type;
        for (int i = kCoffeeType_Americano; i < kCoffeeType_NumTypes; i++)
        {
            if (type == i)
            {
                lv_obj_add_state(coffee_type_imgbtns[i], LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(coffee_type_imgbtns[i], LV_STATE_CHECKED);
            }
        }
        gui_enable_start_button(true);
        UI_WidgetInteraction_Callback();
    }
}

void gui_set_home_coffee_type_str()
{
    _set_label_font(guider_ui.home_label_americano, s_Language);
    lv_label_set_text(guider_ui.home_label_americano, s_CoffeeTypeStr[s_Language][0]);

    _set_label_font(guider_ui.home_label_cappuccino, s_Language);
    lv_label_set_text(guider_ui.home_label_cappuccino, s_CoffeeTypeStr[s_Language][1]);

    _set_label_font(guider_ui.home_label_espresso, s_Language);
    lv_label_set_text(guider_ui.home_label_espresso, s_CoffeeTypeStr[s_Language][2]);

    _set_label_font(guider_ui.home_label_cafelatte, s_Language);
    lv_label_set_text(guider_ui.home_label_cafelatte, s_CoffeeTypeStr[s_Language][3]);
}

void gui_set_home_coffee_size(coffee_size_t size)
{
    if ((size >= kCoffeeSize_Small) && (size <= kCoffeeSize_NumSizes))
    {
        lv_obj_t *coffee_size_imgbtns[kCoffeeSize_NumSizes];
        coffee_size_imgbtns[kCoffeeSize_Small]  = guider_ui.home_imgbtn_size_small;
        coffee_size_imgbtns[kCoffeeSize_Medium] = guider_ui.home_imgbtn_size_medium;
        coffee_size_imgbtns[kCoffeeSize_Large]  = guider_ui.home_imgbtn_size_large;
        s_CoffeeSize                            = size;
        for (int i = kCoffeeSize_Small; i < kCoffeeSize_NumSizes; i++)
        {
            if (s_CoffeeSize == i)
            {
                lv_obj_add_state(coffee_size_imgbtns[i], LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(coffee_size_imgbtns[i], LV_STATE_CHECKED);
            }
        }
        gui_enable_start_button(true);
        UI_WidgetInteraction_Callback();
    }
}

void gui_set_home_coffee_size_str()
{
    _set_label_font_20(guider_ui.home_label_size, s_Language);
    lv_label_set_text(guider_ui.home_label_size, s_CoffeeAttrStr[s_Language][1]);

    _set_label_font(guider_ui.home_label_size_small, s_Language);
    lv_label_set_text(guider_ui.home_label_size_small, s_CoffeeSizeStr[s_Language][0]);

    _set_label_font(guider_ui.home_label_size_medium, s_Language);
    lv_label_set_text(guider_ui.home_label_size_medium, s_CoffeeSizeStr[s_Language][1]);

    _set_label_font(guider_ui.home_label_size_large, s_Language);
    lv_label_set_text(guider_ui.home_label_size_large, s_CoffeeSizeStr[s_Language][2]);
}

void gui_set_home_coffee_strength(coffee_strength_t strength)
{
    if ((strength >= kCoffeeStrength_Soft) && (strength <= kCoffeeStrength_NumStrengths))
    {
        lv_obj_t *coffee_strength_imgbtns[kCoffeeStrength_NumStrengths];
        coffee_strength_imgbtns[kCoffeeStrength_Soft]   = guider_ui.home_imgbtn_strength_soft;
        coffee_strength_imgbtns[kCoffeeStrength_Medium] = guider_ui.home_imgbtn_strength_mild;
        coffee_strength_imgbtns[kCoffeeStrength_Strong] = guider_ui.home_imgbtn_strength_strong;
        s_CoffeeStrength                                = strength;
        for (int i = kCoffeeStrength_Soft; i < kCoffeeStrength_NumStrengths; i++)
        {
            if (s_CoffeeStrength == i)
            {
                lv_obj_add_state(coffee_strength_imgbtns[i], LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(coffee_strength_imgbtns[i], LV_STATE_CHECKED);
            }
        }
        gui_enable_start_button(true);
        UI_WidgetInteraction_Callback();
    }
}

void gui_set_home_coffee_strength_str()
{
    _set_label_font_20(guider_ui.home_label_strength, s_Language);
    if (s_Language == kLanguage_CN)
    {
        lv_obj_set_size(guider_ui.home_label_strength, 65, 25);
    }
    else
    {
        lv_obj_set_size(guider_ui.home_label_strength, 122, 25);
    }
    lv_label_set_text(guider_ui.home_label_strength, s_CoffeeAttrStr[s_Language][2]);

    _set_label_font(guider_ui.home_label_strength_soft, s_Language);
    lv_label_set_text(guider_ui.home_label_strength_soft, s_CoffeeStrengthStr[s_Language][0]);

    _set_label_font(guider_ui.home_label_strength_mild, s_Language);
    lv_label_set_text(guider_ui.home_label_strength_mild, s_CoffeeStrengthStr[s_Language][1]);

    _set_label_font(guider_ui.home_label_strength_strong, s_Language);
    lv_label_set_text(guider_ui.home_label_strength_strong, s_CoffeeStrengthStr[s_Language][2]);
}

void gui_set_home_start_str()
{
    _set_label_font(guider_ui.home_label_start, s_Language);
    lv_label_set_text(guider_ui.home_label_start, s_StartStr[s_Language][0]);
}

void gui_set_finished_coffee_type(lv_ui *ui)
{
    if ((s_CoffeeType >= kCoffeeType_Americano) && (s_CoffeeType < kCoffeeType_NumTypes))
    {
        lv_img_dsc_t **animimgs = NULL;
        switch (s_CoffeeType)
        {
            case kCoffeeType_Americano:
                animimgs = (lv_img_dsc_t **)s_FinishedAnimImagAmericano;
                break;
            case kCoffeeType_Cappuccino:
                animimgs = (lv_img_dsc_t **)s_FinishedAnimImagCappuccino;
                break;
            case kCoffeeType_Espresso:
                animimgs = (lv_img_dsc_t **)s_FinishedAnimImagEspresso;
                break;
            case kCoffeeType_Latte:
                animimgs = (lv_img_dsc_t **)s_FinishedAnimImagCafelatte;
                break;
            default:
                break;
        }
        lv_animimg_set_src(ui->finished_animimg_coffee_type, (lv_img_dsc_t **)animimgs, 1);
        lv_animimg_set_duration(ui->finished_animimg_coffee_type, 3000);
        lv_animimg_t *finished_animimg_americano_ext = (lv_animimg_t *)ui->finished_animimg_coffee_type;
        lv_anim_set_ready_cb(&finished_animimg_americano_ext->anim, gui_finished_anim_ready_cb);
        lv_animimg_set_repeat_count(ui->finished_animimg_coffee_type, 1);
        lv_animimg_start(ui->finished_animimg_coffee_type);
    }
}

void gui_set_standby_animation(lv_ui *ui)
{
    uint8_t animimg_num = sizeof(s_StandbyAnimImgs) / sizeof(lv_img_dsc_t *);
    LVGL_LOG("+gui_set_standby_animation:%d\r\n", animimg_num);
    lv_animimg_set_src(ui->standby_animimg_screensaver, (lv_img_dsc_t **)s_StandbyAnimImgs, animimg_num);
    lv_animimg_set_duration(ui->standby_animimg_screensaver, animimg_num * 800);
    lv_animimg_set_repeat_count(ui->standby_animimg_screensaver, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(ui->standby_animimg_screensaver);
}

void gui_set_standby_help()
{
    _set_label_font(guider_ui.standby_label_voice_ww, s_Language);
    lv_label_set_text(guider_ui.standby_label_voice_ww, s_StandbyHelpStr[s_Language][0]);

    _set_label_font(guider_ui.standby_label_or, s_Language);
    lv_label_set_text(guider_ui.standby_label_or, s_StandbyHelpStr[s_Language][1]);

    _set_label_font(guider_ui.standby_label_touch_ww, s_Language);
    lv_label_set_text(guider_ui.standby_label_touch_ww, s_StandbyHelpStr[s_Language][2]);

    _set_label_font(guider_ui.standby_label_select_coffee, s_Language);
    lv_label_set_text(guider_ui.standby_label_select_coffee, s_StandbyHelpStr[s_Language][3]);
}

static void gui_setup_screen(screen_t id, int delay)
{
    lv_obj_t **scr_to_load = NULL;
    lv_disp_t *d           = lv_obj_get_disp(lv_scr_act());

    LVGL_LOG("+gui_set_screen:[%d] lang:[%d]\r\n", id, s_Language);
    lv_anim_del_all();

    switch (id)
    {
        case kScreen_Home:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.home;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_home(&guider_ui);
                    lv_obj_add_event_cb(guider_ui.home_img_nxpFaceRec, _home_img_nxpFaceRec_event_handler, LV_EVENT_ALL,
                                        NULL);
                    lv_obj_add_flag(guider_ui.home_label_algoinfo, LV_OBJ_FLAG_HIDDEN);
                }
                lv_dropdown_set_selected(guider_ui.home_ddlist_languageSel, s_Language);

                gui_set_home_coffee_type(kCoffeeType_NumTypes);
                gui_set_home_coffee_type_str();
                gui_set_home_coffee_size(kCoffeeSize_NumSizes);
                gui_set_home_coffee_size_str();
                gui_set_home_coffee_strength(kCoffeeStrength_NumStrengths);
                gui_set_home_coffee_strength_str();
                gui_set_home_start_str();

                gui_enable_start_button(false);
            }
        }
        break;

        case kScreen_Brewing:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.brewing;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_brewing(&guider_ui);
                }
                gui_set_brewing_animation(&guider_ui);
                gui_set_coffee_selection(&guider_ui, kScreen_Brewing);
            }
        }
        break;

        case kScreen_Finished:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.finished;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_finished(&guider_ui);
                }
                gui_set_finished_coffee_type(&guider_ui);
                gui_set_coffee_selection(&guider_ui, kScreen_Finished);
            }
        }
        break;

        case kScreen_Standby:
        {
            if (d->prev_scr == NULL && d->scr_to_load == NULL)
            {
                scr_to_load = &guider_ui.standby;
                if (!lv_obj_is_valid(*scr_to_load))
                {
                    setup_scr_standby(&guider_ui);
                    lv_obj_add_event_cb(guider_ui.standby_cont_help, standby_wake_up_event_handler, LV_EVENT_ALL, NULL);
                    lv_obj_add_event_cb(guider_ui.standby_img_nxpVoiceRec, standby_wake_up_event_handler, LV_EVENT_ALL,
                                        NULL);
                    lv_obj_add_event_cb(guider_ui.standby_img_nxpFaceRec, standby_wake_up_event_handler, LV_EVENT_ALL,
                                        NULL);
                }
                lv_dropdown_set_selected(guider_ui.standby_ddlist_languageSel, s_Language);
                gui_set_standby_help();
                gui_set_standby_animation(&guider_ui);
            }
        }
        break;

        default:
            break;
    }

    if (*scr_to_load)
    {
        lv_scr_load_anim(*scr_to_load, LV_SCR_LOAD_ANIM_NONE, 0, delay, false);
        LVGL_LOG("-gui_set_screen:[%d]\r\n", id);
    }
}

void gui_set_home()
{
    if (UI_EnterScreen_Callback(kScreen_Home))
    {
        gui_setup_screen(kScreen_Home, NO_DELAY);
    }
}

void gui_set_brewing()
{
    if (!gui_check_coffee_selection())
    {
        return;
    }

    if (UI_EnterScreen_Callback(kScreen_Brewing))
    {
        gui_setup_screen(kScreen_Brewing, NO_DELAY);
    }
}

void gui_set_finished()
{
    if (UI_EnterScreen_Callback(kScreen_Finished))
    {
        gui_setup_screen(kScreen_Finished, NO_DELAY);
    }
}

void gui_set_standby()
{
    if (UI_EnterScreen_Callback(kScreen_Standby))
    {
        gui_setup_screen(kScreen_Standby, NO_DELAY);
    }
}

void gui_brewing_anim_ready_cb(struct _lv_anim_t *anim)
{
    gui_set_finished();
}

void gui_main_anim_ready_cb(struct _lv_anim_t *anim)
{
    gui_set_standby();
}

void gui_finished_anim_ready_cb(struct _lv_anim_t *anim)
{
    UI_Finished_Callback();
}

void gui_standby_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.standby_ddlist_languageSel);
    language_t new_language = (language_t)opt_id;
    LVGL_LOG("+-gui_standby_language_changed_cb:[%d:%d]\r\n", s_Language, new_language);
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids) && (new_language != s_Language))
    {
        set_language(new_language);
        gui_set_standby_help();
        UI_SetLanguage_Callback(new_language);
    }
}

void gui_home_language_changed_cb(void)
{
    uint16_t opt_id         = lv_dropdown_get_selected(guider_ui.home_ddlist_languageSel);
    language_t new_language = (language_t)opt_id;
    LVGL_LOG("+-gui_home_language_changed_cb:[%d:%d]\r\n", s_Language, new_language);
    if ((new_language >= kLanguage_EN) && (new_language < kLanguage_Ids) && (new_language != s_Language))
    {
        set_language(new_language);
        gui_set_home_coffee_type_str();
        gui_set_home_coffee_size_str();
        gui_set_home_coffee_strength_str();
        gui_set_home_start_str();
        UI_SetLanguage_Callback(new_language);
        UI_WidgetInteraction_Callback();
    }
}

void gui_home_set_language(language_t lang)
{
    if ((lang >= kLanguage_EN) && (lang < kLanguage_Ids) && (lang != s_Language))
    {
        set_language(lang);
        lv_dropdown_set_selected(guider_ui.home_ddlist_languageSel, lang);
        gui_set_home_coffee_type_str();
        gui_set_home_coffee_size_str();
        gui_set_home_coffee_strength_str();
        gui_set_home_start_str();
    }
}

/**
 * @brief enable the deregister button
 *
 */
void gui_enable_deregister_button(bool enable)
{
    if (lv_obj_is_valid(guider_ui.home_imgbtn_unregister))
    {
        if (enable)
        {
            lv_obj_clear_flag(guider_ui.home_imgbtn_unregister, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_add_flag(guider_ui.home_imgbtn_unregister, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/**
 * @brief set the virtual face image widget
 *
 */
void gui_set_virtual_face(lv_img_dsc_t *pImage)
{
    if (lv_obj_is_valid(guider_ui.home_img_cameraPreview))
    {
        lv_img_set_src(guider_ui.home_img_cameraPreview, pImage);
    }
}

void gui_set_algo_debuginfo(uint8_t isDetFace,
                            uint8_t isSmallFace,
                            uint8_t isBlurry,
                            uint8_t isSideface,
                            uint8_t brightness,
                            int recognized,
                            int id)
{
    char labelInfo[20];
    char *curText = lv_label_get_text(guider_ui.home_label_algoinfo);

    if (!isDetFace)
    {
        sprintf(labelInfo, "No Face");
    }
    else if (isSmallFace)
    {
        sprintf(labelInfo, "Small");
    }
    else if (isBlurry)
    {
        sprintf(labelInfo, "Blurry");
    }
    else if (isSideface)
    {
        sprintf(labelInfo, "Side");
    }
    else if (brightness)
    {
        if (brightness > TABLE_DEBUGINFO_BRIGHTNESS_HIGH)
        {
            sprintf(labelInfo, "Light [%d]", brightness);
        }
        else if (brightness < TABLE_DEBUGINFO_BRIGHTNESS_LOW)
        {
            sprintf(labelInfo, "Dark [%d]", brightness);
        }
    }
    else
    {
        if (recognized && (id >= 0))
        {
            sprintf(labelInfo, "User_%02d", id);
        }
        else if (recognized && (id < 0))
        {
            sprintf(labelInfo, "New user");
        }
        else
        {
            sprintf(labelInfo, "Invalid");
        }
    }

    if ((curText != NULL) && strcmp(curText, labelInfo))
    {
        lv_label_set_text(guider_ui.home_label_algoinfo, labelInfo);
    }
}

/**
 * Initializes demo application
 */
void custom_init(lv_ui *ui)
{ /* Add your code here */
#ifdef RT_PLATFORM
    s_LVGLMutex = xSemaphoreCreateMutex();

    if (s_LVGLMutex == NULL)
    {
        while (1)
        {
        }
    }
#endif
}
