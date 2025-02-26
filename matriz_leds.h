#ifndef CONFIG_MATRIZ_H
#define CONFIG_MATRIZ_H

#define NUM_PIXELS 25
// pino de saída da matriz(ws2812) da placa
#define OUT_PIN 7
// variaveis para a matriz
uint32_t valor_led;
double r = 0.0, b = 0.0, g = 0.0;
PIO pio = pio0;
uint offset;
uint sm;
// Inicializa e configura a matriz ws2812 (PIO)
void init_pio_matriz()
{
  offset = pio_add_program(pio, &pio_matrix_program);
  sm = pio_claim_unused_sm(pio, true);
  pio_matrix_program_init(pio, sm, offset, OUT_PIN);
}
// Matrizes com os desenhos que passam um valor específico para cada um dos leds para saber se será acionado ou não
double apagar_leds[NUM_PIXELS] =
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
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
    {0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.1, 0.0,
     0.1, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0};
double num_2[NUM_PIXELS] =
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.0, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1};
double num_3[NUM_PIXELS] =
    {0.0, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.1};
double num_4[NUM_PIXELS] =
    {0.0, 0.1, 0.0, 0.0, 0.1,
     0.1, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.1};
double cheio[NUM_PIXELS] =
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0};

// Rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}
// Rotinas para acionar a matrix de leds na cor vermelha
void desenho_pio_vermelho(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
  for (int16_t i = 0; i < NUM_PIXELS; i++)
  {
    valor_led = matrix_rgb(b = 0.0, desenho[24 - i], g = 0.0);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}
// Rotinas para acionar a matrix de leds na cor verde
void desenho_pio_verde(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
  for (int16_t i = 0; i < NUM_PIXELS; i++)
  {
    valor_led = matrix_rgb(b = 0.0, r = 0.0, desenho[24 - i]);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}
// Aciona o buzzer recebendo um tempo em ms como parâmetro
void liga_buzzer(int duracao)
{
  int tempo = duracao;
  while (tempo > 0)
  {
    gpio_put(BUZZER, true);
    sleep_ms(1);
    tempo--;
    gpio_put(BUZZER, false);
    sleep_ms(3);
    tempo -= 3;
  }
}
// Rotina para piscar em verde a letra V quando o acesso for liberado
void matriz_senha_correta(PIO pio, uint sm)
{
  desenho_pio_verde(letra_V, valor_led, pio, sm, r, g, b);
  sleep_ms(400);
  desenho_pio_verde(apagar_leds, valor_led, pio, sm, r, g, b);
  sleep_ms(150);
}
// Rotina para piscar em vermelho a letra X e acionar o buzzer quando o bloqueio for ativado
void matriz_modo_bloqueio(PIO pio, uint sm)
{
  gpio_put(LED_GREEN, false);

  desenho_pio_vermelho(letra_X, valor_led, pio, sm, r, g, b);
  liga_buzzer(200);
  sleep_ms(350);
  desenho_pio_vermelho(apagar_leds, valor_led, pio, sm, r, g, b);
  sleep_ms(150);
}
// Rotina para gerenciar a matriz de leds durante todo o funcionamento do programa, baseando-se nas VAR id_led, digito_atual e bloqueio 
void gerencia_matriz_leds(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b, int id_led, int digito_atual, int bloqueio)
{
  if (id_led == 0)
  {
    if (digito_atual == 0)
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, num_1[24 - i], num_1[24 - i]); // Configura os leds com sua devida cor e intesidade e altera o estado do led baseando-se no desenho recebido (a matriz)
        pio_sm_put_blocking(pio, sm, valor_led); // Nesse caso o numero 1 será desenhado na matriz
      }
    }
    else if (digito_atual == 1)
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, num_2[24 - i], num_2[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }
    else if (digito_atual == 2)
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, num_3[24 - i], num_3[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }
    else if (digito_atual == 3)
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, num_4[24 - i], num_4[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }
    else if (digito_atual == 4)
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, cheio[24 - i], cheio[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }
    else
    {
      for (int16_t i = 0; i < NUM_PIXELS; i++)
      {
        valor_led = matrix_rgb(b = 0.0, apagar_leds[24 - i], apagar_leds[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
      }
    }
  }
  else if (id_led == 1) // Caso da senha digitada corresponder a senha correta 
  {
    matriz_senha_correta(pio, sm);
  }
  else if (id_led == 2) // Caso da senha digitada NÃO corresponder a senha correta 
  {
    if (bloqueio == 3) // Caso o usuário erre a senha 3 vezes
    {
      matriz_modo_bloqueio(pio, sm);
    }
    else
    {
      desenho_pio_vermelho(letra_X, valor_led, pio, sm, r, g, b);
    }
  }
}
// Altera leds, matriz, display ou aciona buzzer baseando-se nas VAR passadas pelo WEBSERVER
void alegorias_web()
{
  if(acesso_direto== true && block_web == false)
  {
    acesso_direto_display();
    gpio_put(LED_RED, false);
    matriz_senha_correta(pio, sm);
  }
  else if(acesso_direto== false && block_web == true)
  {
    emergencia_display();
    gpio_put(LED_GREEN, false);
    matriz_modo_bloqueio(pio, sm);
  }
}

#endif