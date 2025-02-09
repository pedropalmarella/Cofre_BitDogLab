#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "matriz_leds.h"
#include "pio_matrix.pio.h"
#include "init_GPIO.h"
#include "pico/bootrom.h"

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0; // captura o tempo do ultimo acionamento para o debounce 
const int senha_correta[4] = {1, 1, 2, 2};
int senha[4] = {0, 0, 0, 0}; // senha a ser digitada
int digito_atual = 0; // indice do digito da senha
int acerto = 0; // verifica o numero de digitos certos na senha
int id_led = 0; // define o padrão dos leds e matriz
int bloqueio = 0; // contador de senhas erradas 

int64_t reseta_ciclo_alarm_callback(alarm_id_t id, void *user_data){
    id_led = 0;
    digito_atual = 0;
    gpio_put(LED_RED, true);
    gpio_put(LED_GREEN, true);
    for (int i = 0; i < 4; i++)
    {
        senha[i] = 0;
    }    
    return 0;   
}

void verifica_senhas(){
    acerto = 0;
    for (int i = 0; i < 4; i++) {
        if (senha_correta[i] == senha[i]) {
            acerto++;
        }
    }
    if (acerto == 4) {
        bloqueio = 0;
        gpio_put(LED_RED, false);
        id_led = 3;
        add_alarm_in_ms(6000, reseta_ciclo_alarm_callback, NULL, false);
    }
    else if(acerto != 4) {
        bloqueio++;
        gpio_put(LED_GREEN, false);
        id_led = 4;
        if(bloqueio == 3){
            add_alarm_in_ms(6000, reseta_ciclo_alarm_callback, NULL, false);
        }
        else{
            add_alarm_in_ms(2500, reseta_ciclo_alarm_callback, NULL, false);
        }
    }
}

// Função de interrupção para todos os botões
void btns_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());// captura o momento do acionamento e converte para microsegundos
    // Verifica o debounce
    if (current_time - last_time > DEBOUNCE_TIME) {
        if (gpio == BTN_A) {
            senha[digito_atual] = 1;
            id_led = 1; 
            digito_atual++;
        } 
        else if (gpio == BTN_B) {
            senha[digito_atual] = 2;
            id_led = 2; 
            digito_atual++;
        } 
        else if (gpio == BTN_STICK) {
            verifica_senhas(); 
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    }
}

int main() {
    init_gpio();
    PIO pio = pio0;

    // Configuração da PIO, necessario para matriz
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Configuração das interrupções para todos os botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, btns_callback);

    while (true) {    
        if(!gpio_get(BTN_A) && !gpio_get(BTN_B)){
            reset_usb_boot(0,0);
        }
        if (id_led == 1) {
            desenho_pio_amarelo(num_1, valor_led, pio, sm, r, g, b);
        } 
        else if (id_led == 2) {
            desenho_pio_amarelo(num_2, valor_led, pio, sm, r, g, b);
        }
        else if (id_led == 3) {  
            matriz_senha_correta(pio, sm); 
        }
        else if (id_led == 4) {
            desenho_pio_vermelho(letra_X, valor_led, pio, sm, r, g, b);
            if(bloqueio == 3)
            {
                modo_bloqueio(pio, sm);
            }
        }   
        else {
            //matriz_modo_espera(pio, sm);   
            desenho_pio_amarelo(apagar_leds, valor_led, pio, sm, r, g, b);            
        }    
        sleep_ms(10); // Pequeno delay para estabilidade
    }
}
