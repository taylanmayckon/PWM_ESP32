#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO (5) // A GPIO pra gerar o PWM
#define LEDC_CHANNEL LEDC_CHANNEL_0 
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Resolução do PWM a ser gerado (VALOR DO TOP)
#define TOP_PWM 8191 // Definindo o TOP do PWM 
#define LEDC_FREQUENCY (4000) // Freq do PWM em Hz


void app_main(void){
    // Configurando o timer do LEDC PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Configurando o canal PWM a ser usado
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    float duty_percent = 0.0;
    bool rising = true;

    while(true){
        if(rising){
            duty_percent+=0.01;
            if(duty_percent >= 1.0f){
                rising = false;
            }
        }
        else{
            duty_percent-=0.01;
            if(duty_percent <= 0.0f){
                rising = true;
            }
        }

        // Ajustando o duty cycle
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, TOP_PWM*duty_percent)); // Seta o Duty Cycle
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL)); // Atualiza o PWM

        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}