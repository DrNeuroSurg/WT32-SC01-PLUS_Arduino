/**
 * @author DrNeurosurg
 * @copyright GPLv3
 */

/* ***********************************************

    by DrNeurosurg 2026

    THIS EXAMPLE - ESP. WT32_SCO1_PLUS.h - IS FOR THIS (WT32-SC01-PLUS) BOARD (ONLY)

    Libraries used (Arduino):

    - Arduino IDE Version: 2.3.9

    - LovyanGFX by lovyan03 Version: 1.2.21

    - lvgl by kisvegabor Version: 9.5.0

    Licensed under MIT license
      
*********************************************** */

#pragma once

#include <Arduino.h>
#include <lvgl.h>

#define WT_USE_LVGL
#include "WT32_SC01_PLUS.h"

// ******************** LVGL **************************************
SemaphoreHandle_t lvgl_mux;
//FOR vTask_delay
uint32_t ms;

// ****************** LVGL  TASK **********************************

#define LVGL_TASK_CORE 1
#define LVGL_TASK_PRIO  5

TaskHandle_t Task_lvgl;

void lvglTask(void *parameter) {
  while(true) {
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    ms = lv_timer_handler();
    xSemaphoreGiveRecursive(lvgl_mux);
    vTaskDelay(pdMS_TO_TICKS(ms ));
  }
}

void createLVGL_Task() {
  xTaskCreatePinnedToCore(
        lvglTask,               /* Function to implement the task */
        "lvglTask",             /* Name of the task */
        10000,                  /* Stack size in words */
        NULL,                   /* Task input parameter */
        LVGL_TASK_PRIO,         /* Priority of the task */
        &Task_lvgl,             /* Task handle. */
        LVGL_TASK_CORE          /* Core where the task should run */
    );
}

void lvglTaskDelete() {
  vTaskDelete(Task_lvgl);
}
//-----------------------------------------------------------------------------

void initGUI() {
  /*Change the active screen's background color*/
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);
  /*Create a white label, set its text and align it to the center*/
  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

//----------------------------------------------------------------------------------------------------

void setup() {

  //Serial.begin(115200);                       // IF YOU NEED IT (FOR DEBUGGIN..)

  // LVGL
  lvgl_mux = xSemaphoreCreateRecursiveMutex();  // CREATE MUTEX !!
  init_display();                               // DO THIS AS EARLY AS POSSIBLE (MEMORY ALLOCATION) !!
  lv_tick_set_cb(xTaskGetTickCount);            // LVGL TICK CALLBACK
  // START LVGL TASK
  createLVGL_Task();

  // HERE WE GO...
  initGUI();

}

void loop() {
  // NOTHING TO DO (YET) !!!!
}
