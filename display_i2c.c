#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "init_i2c.h"
#include "matriz_leds.h"
#include "init_GPIO.h"

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;      // captura o tempo do ultimo acionamento do botão para o debounce
bool verde_on = 0;           // verifica qual led está ligado, 1 -> VERDE, 0 -> AZUL

// Função de interrupção para todos os botões
void btns_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if (current_time - last_time > DEBOUNCE_TIME)    // Verifica se ja passou o tempo do debounce para acionamento novamente do botao
    {
        if (gpio == BTN_A)
        {
            resposta_btns(!gpio_get(LED_GREEN), verde_on = true); //func para acionar o led VERDE, imprimir no terminal e atualizar o display
        }
        else if (gpio == BTN_B)
        {
            resposta_btns(!gpio_get(LED_BLUE), verde_on = false); //func para acionar o led AZUL, imprimir no terminal e atualizar o display
        }
        else if (gpio == BTN_STICK)
        {
            reset_usb_boot(0, 0); //func para entrar no modo bootsel 
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    }
}

int main()
{   
    stdio_init_all();
    init_gpio(); //inicializa todos os GPIOs
    init_pio_matriz(); // inicializa a matriz de leds
    init_i2c(); //inicializa o display
    //selecione a func e pressione f12 para ir até onde a mesma é criada
    // Configuração das interrupções para todos os botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, btns_callback);

    while (true)
    {
        if (stdio_usb_connected()) // Certifica-se de que o USB está conectado
        { 
            char c;
            if (scanf("%c", &c) == 1) // Lê caractere da entrada padrão
            { 
                printf("Caractere recebido: %c\n", c);
                switch (c)
                {
                case '0':
                    liga_matriz(num_0, valor_led, pio, sm, r, g, b); // liga a matriz de leds, é a mesma func apenas alterando o frame(desenho) a ser apresentado
                    desenha_caractere(c);
                    break;
                case '1':
                    liga_matriz(num_1, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '2':
                    liga_matriz(num_2, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '3':
                    liga_matriz(num_3, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '4':
                    liga_matriz(num_4, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '5':
                    liga_matriz(num_5, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '6':
                    liga_matriz(num_6, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '7':
                    liga_matriz(num_7, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '8':
                    liga_matriz(num_8, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                case '9':
                    liga_matriz(num_9, valor_led, pio, sm, r, g, b);
                    desenha_caractere(c);
                    break;
                default:
                    desenha_caractere(c); //func para atualizar o display com o caractere digitado
                    liga_matriz(apagar_leds, valor_led, pio, sm, r, g, b);
                }
            }
        }
        sleep_ms(10); // Pequeno delay para estabilidade
    }
}
