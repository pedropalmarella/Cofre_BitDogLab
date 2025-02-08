#ifndef CONFIG_MATRIZ_H
#define CONFIG_MATRIZ_H

#include "pico/stdlib.h"
#include "pio_matrix.pio.h"
#include "init_GPIO.h"

#define NUM_PIXELS 25
// pino de saída da matriz(ws2812) da placa
#define OUT_PIN 7
// variaveis para inciar a matriz
uint32_t valor_led;
double r = 0.0, b = 0.0, g = 0.0;
PIO pio = pio0;
uint offset;
uint sm;

void init_pio_matriz()
{
  // Configuração da PIO, necessario para matriz
  offset = pio_add_program(pio, &pio_matrix_program);
  sm = pio_claim_unused_sm(pio, true);
  pio_matrix_program_init(pio, sm, offset, OUT_PIN);
}

double apagar_leds[NUM_PIXELS] =
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0};
double num_0[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.0, 0.35, 0.35, 0.35, 0.0};
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
double num_3[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35};
double num_4[NUM_PIXELS] =
    {0.0, 0.35, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.35, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35};
double num_5[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.35, 0.35, 0.35, 0.35, 0.35};
double num_6[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35};
double num_7[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.35};
double num_8[NUM_PIXELS] =
    {0.0, 0.35, 0.35, 0.35, 0.0,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.0, 0.35, 0.35, 0.35, 0.0};
double num_9[NUM_PIXELS] =
    {0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.35,
     0.35, 0.35, 0.35, 0.35, 0.35,
     0.35, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.35, 0.35, 0.35, 0.35};

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// rotinas para acionar a matrix de leds - ws2812b
void liga_matriz(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
 for (int16_t i = 0; i < NUM_PIXELS; i++)
  {

    valor_led = matrix_rgb(desenho[24 - i], r = 0.0, g = 0.0);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}

#endif