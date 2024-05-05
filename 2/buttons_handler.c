/**
 * @file main.c
 * @author Dmitrii Fedin
 * @brief Шаблон задачи по RTOS
 * @version 0.1
 * @date 2022-01-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


enum BUTTONS {
    OK_BUTTON, //пин 0
    CANCEL_BUTTON, //пин 1
    PREV_BUTTON, //пин 2
    NEXT_BUTTON //пин 3
};

char *stringsButtons[] = {
    "OK",
    "CANCEL",
    "PREV",
    "NEXT"
};


/**
 * @brief Эмуляция прерывания GPIO
 * 
 * @param pin_no номер пина. Может принимать значения от 0 до 3. 
 *
 */
void gpio_IRQ(uint8_t pin_no) {
    printf("%d\n", stringsButtons[pin_no]);
}


/**
 * @brief Задача, которая должна обрабатывать нажатия кнопок
 * 
 * Типы нажатий, которые могут обрабытываться 
 *  - одиночные
 *  - зажаты 2 кнопки. К примеру, OK и PREV
 * 
 *  В качестве действия можно использовать printf()
 * 
 * @param arg 
 */
void buttons_process_task(void* arg) {
    for(;;) {
        for(int i = 0; i < 4; ++i) {
            bool test = gpio_read(i);
            if (test == true) {
                gpio_IRQ(i);
            }
        }

        vTaskDelay( 500 / portTICK_PERIOD_MS ); //задержка чтобы не считывать одно нажатие кнопки несколько раз
    }
    vTaskDelete(NULL);
}


/**
 * @brief Функция для запуска 
 * 
 */
void start_buttons_processing() {
    
    //якобы создаём задачу
    xTaskCreate(buttons_process_task, "buttons_task", 1024*4, NULL, 1, NULL);

    //якобы включаем прерывания
    enableGPIO_IRQ();

    //можно считать, что прерывания можно останавливать, если необходимо
    //disableGPIO_IRQ();
}

//дополнительно доступные функции, которыми можно пользоваться при необходимости
bool gpio_read(uint8_t pin_no);
void enableGPIO_IRQ();
void disableGPIO_IRQ();