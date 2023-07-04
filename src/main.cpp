#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "config.hpp"

#include <Adafruit_LvGL_Glue.h> // Always include this BEFORE lvgl.h!
#include <lvgl.h>

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI1, TFT_CS, TFT_DC, TFT_RST);
Adafruit_LvGL_Glue glue;

void setupSPI()
{
    SPI1.setTX(TFT_MOSI);
    SPI1.setSCK(TFT_SCLK);
    SPI1.setCS(TFT_CS);
    SPI1.begin();
}

lv_obj_t * btn;
lv_obj_t * btn2;

static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_style_t style_btn_red;

static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    return lv_color_darken(color, opa);
}

static void style_init(void)
{
    /*Create a simple button style*/
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn, lv_color_black());
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_black());

    /*Create a style for the pressed state.
     *Use a color filter to simply modify all colors in this state*/
    static lv_color_filter_dsc_t color_filter;
    lv_color_filter_dsc_init(&color_filter, darken);
    lv_style_init(&style_btn_pressed);
    lv_style_set_color_filter_dsc(&style_btn_pressed, &color_filter);
    lv_style_set_color_filter_opa(&style_btn_pressed, LV_OPA_20);

    /*Create a red style. Change only some colors.*/
    lv_style_init(&style_btn_red);
    lv_style_set_bg_color(&style_btn_red, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_btn_red, lv_palette_lighten(LV_PALETTE_RED, 3));
}

void lvgl_setup(void)
{
    /*Initialize the style*/
    style_init();

    /*Create a button and use the new styles*/
    btn = lv_btn_create(lv_scr_act());
    /* Remove the styles coming from the theme
     * Note that size and position are also stored as style properties
     * so lv_obj_remove_style_all will remove the set size and position too */
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 80, 50);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -5,5);

    /*Add a label to the button*/
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "A");
    lv_obj_center(label);

    /*Create another button and use the red style too*/
    btn2 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn2);                      /*Remove the styles coming from the theme*/
    lv_obj_set_size(btn2, 80, 50);
    lv_obj_add_style(btn2, &style_btn, 0);
    lv_obj_add_style(btn2, &style_btn_red, 0);
    lv_obj_add_style(btn2, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn2, LV_RADIUS_CIRCLE, 0); /*Add a local style too*/
    lv_obj_align_to(btn2, btn, LV_ALIGN_OUT_BOTTOM_MID,0,20);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "B");
    lv_obj_center(label);
}

void setup(void)
{
    Serial.begin(115200);
    delay(1000);
    setupSPI();

    Serial.println("INIT");

    pinMode(PIN_STICK_DOWN, INPUT_PULLUP);
    pinMode(PIN_STICK_UP, INPUT_PULLUP);
    pinMode(PIN_STICK_LEFT, INPUT_PULLUP);
    pinMode(PIN_STICK_RIGHT, INPUT_PULLUP);
    pinMode(PIN_STICK_PRESS, INPUT_PULLUP);

    pinMode(PIN_BUTTON_A, INPUT_PULLUP);
    pinMode(PIN_BUTTON_B, INPUT_PULLUP);

    tft.init(135, 240);
    tft.setRotation(3);
    tft.fillScreen(ST77XX_BLACK);
    // tft.setSPISpeed(60000000);

    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);

    LvGLStatus status = glue.begin(&tft, false);

    /*

    if (status != LVGL_OK)
    {
        Serial.printf("Glue error %d\r\n", (int)status);
        for (;;)
            ;
    }
    */

    lvgl_setup(); // Call UI-building function above
}

void loop()
{
    if (digitalRead(PIN_STICK_DOWN) == LOW)
    {
        Serial.println("DOWN");

        while (digitalRead(PIN_STICK_DOWN) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_UP) == LOW)
    {
        Serial.println("UP");

        while (digitalRead(PIN_STICK_UP) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_LEFT) == LOW)
    {
        Serial.println("LEFT");

        while (digitalRead(PIN_STICK_LEFT) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_RIGHT) == LOW)
    {
        Serial.println("RIGHT");

        while (digitalRead(PIN_STICK_RIGHT) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_PRESS) == LOW)
    {
        Serial.println("PRESS");
        while (digitalRead(PIN_STICK_PRESS) == LOW)
        {
            delay(50);
        }
    }

    if (digitalRead(PIN_BUTTON_A) == LOW)
    {
        Serial.println("A");

        lv_event_send(btn, LV_EVENT_PRESSED,NULL );
        while (digitalRead(PIN_BUTTON_A) == LOW)
        {
            lv_task_handler(); // Call LittleVGL task handler periodically
            lv_tick_inc(10);
            delay(50);
        }
        lv_event_send(btn, LV_EVENT_RELEASED,NULL );
    }
    if (digitalRead(PIN_BUTTON_B) == LOW)
    {
        Serial.println("B");

        lv_event_send(btn2, LV_EVENT_PRESSED,NULL );
        while (digitalRead(PIN_BUTTON_B) == LOW)
        {
            lv_task_handler(); // Call LittleVGL task handler periodically
            lv_tick_inc(10);
            delay(50);
        }
        lv_event_send(btn2, LV_EVENT_RELEASED,NULL );
    }

    lv_task_handler(); // Call LittleVGL task handler periodically
    lv_tick_inc(1);
}
