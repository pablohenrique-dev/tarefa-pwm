#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO 22   // GPIO para o servo
#define LED_RGB 12 // GPIO do LED RGB (BitDogLab)

uint slice;

// Configuração do PWM para o servo
void pwm_setup()
{
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(SERVO);
    pwm_set_clkdiv(slice, 250); // Define o divisor do clock do PWM
    pwm_set_wrap(slice, 9999);  // Define o valor de wrap para obter 50Hz
    pwm_set_enabled(slice, true);
}

// Função para definir o ciclo ativo do servo
void set_servo_position(uint16_t pulse_width)
{
    pwm_set_gpio_level(SERVO, pulse_width);
}

// Função para movimentação suave do servo
void smooth_movement(uint16_t start, uint16_t end)
{
    int step = (start < end) ? 5 : -5;

    for (uint16_t pos = start; (step > 0) ? (pos <= end) : (pos >= end); pos += step)
    {
        set_servo_position(pos);
        sleep_ms(10); // Atraso de ajuste
    }
}

int main()
{
    stdio_init_all();
    pwm_setup();

    // Configuração do LED RGB
    gpio_init(LED_RGB);
    gpio_set_dir(LED_RGB, GPIO_OUT);

    // 1) Posição de 180° (2400µs)
    set_servo_position(2400);
    gpio_put(LED_RGB, 1); // Liga o LED
    sleep_ms(5000);

    // 2) Posição de 90° (1470µs)
    set_servo_position(1470);
    gpio_put(LED_RGB, 0); // Desliga o LED
    sleep_ms(5000);

    // 3) Posição de 0° (500µs)
    set_servo_position(500);
    gpio_put(LED_RGB, 1); // Liga novamente o LED
    sleep_ms(5000);

    // 4) Movimentação periódica entre 0° e 180°
    while (true)
    {
        smooth_movement(500, 2400); // 0° → 180°
        smooth_movement(2400, 500); // 180° → 0°
    }

    return 0;
}
