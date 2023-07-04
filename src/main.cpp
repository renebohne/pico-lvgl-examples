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

void lvgl_setup(void)
{
    lv_obj_t *img = lv_img_create(lv_scr_act());
    LV_IMG_DECLARE(digitale_dinge_logo_kreis);
    lv_img_set_src(img, &digitale_dinge_logo_kreis);
    // lv_obj_align(img, LV_ALIGN_LEFT_MID, 5, 0);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
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

        while (digitalRead(PIN_BUTTON_A) == LOW)
        {
            lv_task_handler(); // Call LittleVGL task handler periodically
            lv_tick_inc(10);
            delay(50);
        }
    }
    if (digitalRead(PIN_BUTTON_B) == LOW)
    {
        Serial.println("B");

        while (digitalRead(PIN_BUTTON_B) == LOW)
        {
            lv_task_handler(); // Call LittleVGL task handler periodically
            lv_tick_inc(10);
            delay(50);
        }
    }

    lv_task_handler(); // Call LittleVGL task handler periodically
    lv_tick_inc(1);
}
