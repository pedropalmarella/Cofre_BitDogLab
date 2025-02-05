#ifndef CONFIG_MATRIZ_H  
#define CONFIG_MATRIZ_H

#include "pico/stdlib.h"
#include "pio_matrix.pio.h"
#include "init_GPIO.h"

#define NUM_PIXELS 25
//pino de saída da matriz(ws2812) da placa 
#define OUT_PIN 7
//variaveis para a matriz
uint32_t valor_led;
double r = 0.0, b = 0.0 , g = 0.0;

double apagar_leds[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};                      
double loading1[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.2, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double loading2[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.2, 0.2, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double loading3[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.2, 0.2, 0.2, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double loading4[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.2, 0.2, 0.2, 0.2, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double loading5[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.2, 0.2, 0.2, 0.2, 0.2,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double volta1[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.2,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double volta2[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.2, 0.2,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double volta3[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.2, 0.2, 0.2,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double volta4[NUM_PIXELS] = 
                    { 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.2, 0.2, 0.2, 0.2,
                      0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
double letra_V[NUM_PIXELS] = 
                     {0.35, 0.0, 0.0, 0.0, 0.35,
                      0.0, 0.35, 0.0, 0.0, 0.35,
                      0.35, 0.0, 0.35, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.35, 0.35,
                      0.35, 0.0, 0.0, 0.0, 0.0};
double letra_X[NUM_PIXELS] = 
                     {0.35, 0.0, 0.0, 0.0, 0.35,
                      0.0, 0.35, 0.0, 0.35, 0.0,
                      0.0, 0.0, 0.35, 0.0, 0.0,
                      0.0, 0.35, 0.0, 0.35, 0.0,
                      0.35, 0.0, 0.0, 0.0, 0.35};     
double num_1[NUM_PIXELS] =   
                        {0.0, 0.0, 0.2, 0.0, 0.0,
                         0.0, 0.0, 0.2, 0.2, 0.0, 
                         0.2, 0.0, 0.2, 0.0, 0.0,
                         0.0, 0.0, 0.2, 0.0, 0.0,
                         0.0, 0.2, 0.2, 0.2, 0.0};
double num_2[NUM_PIXELS] =   
                        {0.2, 0.2, 0.2, 0.2, 0.2,
                         0.2, 0.0, 0.0, 0.0, 0.0, 
                         0.2, 0.2, 0.2, 0.2, 0.2,
                         0.0, 0.0, 0.0, 0.0, 0.2,
                         0.2, 0.2, 0.2, 0.2, 0.2};


//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotinas para acionar a matrix de leds - ws2812b
void desenho_pio_vermelho(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        
            valor_led = matrix_rgb(b=0.0, desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void desenho_pio_verde(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        
            valor_led = matrix_rgb(b=0.0, r=0.0, desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void desenho_pio_amarelo(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        
            valor_led = matrix_rgb(b=0.0, desenho[24-i], desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void matriz_modo_espera(PIO pio, uint sm){
    desenho_pio_amarelo(loading1, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(loading2, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(loading3, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(loading4, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(loading5, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(volta4, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(volta3, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(volta2, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
    desenho_pio_amarelo(volta1, valor_led, pio, sm, r, g, b);
    sleep_ms(200);
}

void liga_buzzer(int duracao){
    int tempo = duracao;
    while(tempo > 0){
        gpio_put(BUZZER, true);
        sleep_ms(1);
        tempo--;
        gpio_put(BUZZER, false);
        sleep_ms(3);
        tempo -= 3;
    }
}

void matriz_senha_correta(PIO pio, uint sm){
    for (size_t i = 0; i < 9; i++)
    {
      desenho_pio_verde(letra_V, valor_led, pio, sm, r, g, b);
      liga_buzzer(200);
      sleep_ms(350);
      desenho_pio_verde(apagar_leds, valor_led, pio, sm, r, g, b);
      sleep_ms(150);
    }
}

void modo_bloqueio(PIO pio, uint sm){
  gpio_put(LED_GREEN, false);
    for (int i = 0; i < 9; i++)
    {
      desenho_pio_vermelho(letra_X, valor_led, pio, sm, r, g, b);
      liga_buzzer(200);
      sleep_ms(350);
      desenho_pio_vermelho(apagar_leds, valor_led, pio, sm, r, g, b);
      sleep_ms(150);
    }
}


#endif