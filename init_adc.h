#ifndef INIT_ADC_H
#define INIT_ADC_H

#include "hardware/adc.h"
// Definição dos pinos usados para o joystick e LEDs
#define EIXO_X 27       // Pino de leitura do eixo X do joystick (conectado ao ADC)
#define EIXO_Y 26       // Pino de leitura do eixo Y do joystick (conectado ao ADC)
#define ADC_CHANNEL_0 0 // Canal ADC para o eixo X do joystick
#define ADC_CHANNEL_1 1 // Canal ADC para o eixo Y do joystick
#define SWITCH 22       // Pino de leitura do botão do joystick
uint16_t valor_X, valor_Y;

// Função para configurar o joystick e botões
void setup_joystick()
{
    // Inicializa o ADC e os pinos de entrada analógica
    adc_init();            // Inicializa o módulo ADC
    adc_gpio_init(EIXO_X); // Configura o pino 27 (eixo X) para entrada ADC
    adc_gpio_init(EIXO_Y); // Configura o pino 26 (eixo Y) para entrada ADC

    gpio_init(SWITCH);             
    gpio_set_dir(SWITCH, GPIO_IN); 
    gpio_pull_up(SWITCH);          
}
// Observe que os eixos estão invertidos em relção ao DISPLAY
// Função para ler os valores do eixo X do joystick 
void leitura_joystick_X(uint16_t *valor_X)
{
    // Leitura do valor do eixo X do joystick
    adc_select_input(ADC_CHANNEL_0);
    sleep_us(2);                     
    *valor_X = adc_read();                      
}

// Função para ler os valores do eixo Y do joystick 
void leitura_joystick_Y(uint16_t *valor_Y)
{          
    // Leitura do valor do eixo Y do joystick
    adc_select_input(ADC_CHANNEL_1); 
    sleep_us(2);                     
    *valor_Y = adc_read();           
}

#endif