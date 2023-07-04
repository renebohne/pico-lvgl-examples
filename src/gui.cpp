
#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>

#include "gui.h"
using namespace std;

static lv_obj_t *mainBar = nullptr;

class StatusBar
{
    typedef struct
    {
        bool vaild;
        lv_obj_t *icon;
    } lv_status_bar_t;

public:
    StatusBar()
    {
        memset(_array, 0, sizeof(_array));
    }
    void createIcons(lv_obj_t *par)
    {
        lv_obj_t *_bar = lv_obj_create(par);
        lv_obj_set_size(_bar, LV_HOR_RES, _barHeight);

        static lv_style_t barStyle;
        lv_style_init(&barStyle);
        lv_style_set_border_width(&barStyle, 0);
        lv_obj_add_style(_bar, &barStyle, 0);

        _array[0].icon = lv_label_create(_bar);
        lv_label_set_text(_array[0].icon, " ");

        _array[1].icon = lv_label_create(_bar);
        lv_img_set_src(_array[1].icon, LV_SYMBOL_BATTERY_FULL);

        _array[2].icon = lv_label_create(_bar);
        lv_img_set_src(_array[2].icon, LV_SYMBOL_GPS);
        lv_obj_add_flag(_array[2].icon, LV_OBJ_FLAG_HIDDEN);

        _array[3].icon = lv_label_create(_bar);
        lv_img_set_src(_array[3].icon, LV_SYMBOL_BLUETOOTH);
        lv_obj_add_flag(_array[3].icon, LV_OBJ_FLAG_HIDDEN);

        refresh();
    }

    void setStepCounter(uint32_t counter)
    {
        lv_label_set_text(_array[5].icon, String(counter).c_str());
        lv_obj_align(_array[5].icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }

    void updateLevel(int level)
    {
        lv_label_set_text(_array[0].icon, (String(level) + "%").c_str());
        refresh();
    }

    void updateBatteryIcon(lv_icon_battery_t icon)
    {
        const char *icons[6] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};
        lv_img_set_src(_array[1].icon, icons[icon]);
        refresh();
    }

    void show(int index)
    {
        lv_obj_clear_flag(_array[index].icon, LV_OBJ_FLAG_HIDDEN);
        // TODO  refresh crash
        refresh();
    }

    void hide(int index)
    {
        lv_obj_add_flag(_array[index].icon, LV_OBJ_FLAG_HIDDEN);
        refresh();
    }
    uint8_t height()
    {
        return _barHeight;
    }
    lv_obj_t *self()
    {
        return _bar;
    }

private:
    void refresh()
    {
        int prev = 0;
        for (int i = 0; i < 4; i++)
        {
            if (!lv_obj_has_flag(_array[i].icon, LV_OBJ_FLAG_HIDDEN))
            {
                if (i == LV_STATUS_BAR_BATTERY_LEVEL)
                {
                    lv_obj_align(_array[i].icon, LV_ALIGN_RIGHT_MID, 0, 0);
                }
                else
                {
                    lv_obj_align_to(_array[i].icon, _array[prev].icon, LV_ALIGN_OUT_LEFT_MID, iconOffset, 0);
                }
                prev = i;
            }
        }
    };
    lv_obj_t *_bar = nullptr;
    lv_obj_t *_par = nullptr;
    uint8_t _barHeight = 30;
    lv_status_bar_t _array[6];
    const int8_t iconOffset = -5;
};

StatusBar bar;

void setBluetoothIcon(bool bluetoothIsOn)
{
    if (bluetoothIsOn)
    {
        bar.show(3);
    }
    else
    {
        bar.hide(3);
    }
}

void setGPSIcon(bool gpsIsOn)
{
    if (gpsIsOn)
    {
        bar.show(2);
    }
    else
    {
        bar.hide(2);
    }
}

void updateBatteryLevel(int voltage)
{
    lv_icon_battery_t icon = LV_ICON_BAT_EMPTY;
    if (voltage > 3700)
    {
        icon = LV_ICON_BAT_FULL;
    }
    else if (voltage > 3400)
    {
        icon = LV_ICON_BAT_3;
    }
    else if (voltage > 3100)
    {
        icon = LV_ICON_BAT_2;
    }
    else if (voltage > 2900)
    {
        icon = LV_ICON_BAT_1;
    }

    bar.updateBatteryIcon(icon);
}

void setupGui()
{
    bar.createIcons(lv_scr_act());

    mainBar = lv_obj_create(lv_scr_act());

    lv_style_t mainStyle;
    lv_style_init(&mainStyle);
    lv_style_set_bg_color(&mainStyle, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_opa(&mainStyle, LV_OPA_COVER);
    lv_style_set_radius(&mainStyle, 0);
    lv_obj_add_style(mainBar, &mainStyle, 0);

    lv_obj_set_size(mainBar, 240, 135 - bar.height() - 10);
    // lv_obj_set_size(mainBar,  238, 135 - 30);
    lv_obj_align_to(mainBar, bar.self(), LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}
