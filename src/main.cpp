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

lv_obj_t *tab1;
lv_obj_t *tab2;
lv_obj_t *tab3;
lv_obj_t *tabview;
int currentTab = 2;

void lvgl_setup(void)
{
    /*Create a Tab view object*/

    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 50);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tab1 = lv_tabview_add_tab(tabview, "Tab 1");
    tab2 = lv_tabview_add_tab(tabview, "Tab 2");
    tab3 = lv_tabview_add_tab(tabview, "Tab 3");

    /*Add content to the tabs*/
    lv_obj_t *label = lv_label_create(tab1);
    lv_label_set_text(label, "This the first tab\n\n"
                             "If the content\n"
                             "of a tab\n"
                             "becomes too\n"
                             "longer\n"
                             "than the\n"
                             "container\n"
                             "then it\n"
                             "automatically\n"
                             "becomes\n"
                             "scrollable.\n"
                             "\n"
                             "\n"
                             "\n"
                             "Can you see it?");

    label = lv_label_create(tab2);
    lv_label_set_text(label, "Second tab");

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    lv_obj_scroll_to_view_recursive(label, LV_ANIM_ON);
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
        if (currentTab == 0)
        {
            lv_obj_scroll_by_bounded(tab1, 0, -20, LV_ANIM_ON);
        }

        while (digitalRead(PIN_STICK_DOWN) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_UP) == LOW)
    {
        Serial.println("UP");

        if (currentTab == 0)
        {
            lv_obj_scroll_by_bounded(tab1, 0, 20, LV_ANIM_ON);
        }

        while (digitalRead(PIN_STICK_UP) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_LEFT) == LOW)
    {
        Serial.println("LEFT");
        currentTab--;
        if (currentTab < 1)
        {
            currentTab = 0;
        }
        lv_tabview_set_act(tabview, currentTab, LV_ANIM_ON);

        while (digitalRead(PIN_STICK_LEFT) == LOW)
        {
            delay(50);
        }
    }
    else if (digitalRead(PIN_STICK_RIGHT) == LOW)
    {
        Serial.println("RIGHT");
        currentTab++;
        if (currentTab > 2)
        {
            currentTab = 2;
        }
        lv_tabview_set_act(tabview, currentTab, LV_ANIM_ON);
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

    lv_task_handler(); // Call LittleVGL task handler periodically

    lv_tick_inc(1);
}
