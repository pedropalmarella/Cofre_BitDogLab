#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "pio_matrix.pio.h"
#include "pico/bootrom.h"
#include "inc/font.h"
#include "inc/ssd1306.h"
#include "setup_display.h"
#include "init_adc.h"
#include "init_GPIO.h"
#include "matriz_leds.h"
#include <string.h>
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
////////////////////////////////////////////////////////////

#define WIFI_SSID "Vanda"        // NOME DA REDE WIFI
#define WIFI_PASSWORD "ma1g2no3" // SENHA DA REDE WIFI
// FLAGS PARA ALTERAR O ESTADO DOS BOTÕES NAS INTERRUPÇÕES
volatile bool btnA_pressionado = false;
volatile bool btnB_pressionado = false;
volatile bool switch_pressionado = false;
// INICIALIZA, DEFINE O STATION MODE E CONECTA NA REDE WIFI E INICIALIZA O WEBSERVER
int inicia_web()
{
    conectando_rede_display(); // Display de carregamento
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    // LOOP ATÉ CONECTAR NO WIFI
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0)
    {
        printf("Tentando Conectar...\n");
    }
    printf("Conectado! \n");
    // OPTEI POR INICIAR O LED DO WIFI COMO ON PARA UMA RESPOSTA VISUAL QUE O SISTEMA FOI INICIALIZADO
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    // INICIALIZANDO O WEB SERVER
    httpd_init();
    printf("Http server inicializado\n");
    // CONFIGURANDO E CHAMANDO OS "HANDLERS" DO SSI E CGI
    ssi_init();
    printf("SSI Handler inicializado\n");
    cgi_init();
    printf("CGI Handler inicializado\n");
    // APÓS TUDO TER SIDO INICIALIZADO E CONFIGURADO, O IP DO SERVER É IMPRESSO PARA FACILITAR O ACESSO
    printf("IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));
    options_menu(); // Display do menu principal
}
// REINICIA OU ALTERA VARIÁVEIS E O SISTEMA APÓS UMA AÇÃO "FINAL" COMO O ACIONAMENTO DO SWITCH
int64_t reseta_ciclo_alarm_callback(alarm_id_t id, void *user_data)
{
    // Verifica em caso de bloqueio, criação de senha ou acerto de senha e reinicia as variaveis/display alteradas nesses processos
    if (bloqueio == 3 || criando_senha == true || acerto == 4)
    {
        bloqueio = 0;
        digito_atual = 5;
        habilitar_menu = true;
        menu_option = 1;
        init_i2c();
        options_menu();
    }
    // No caso do usuário ter errado a senha, permanece na tela de inserção e tem mais uma tentativa
    else
    {
        digito_atual = 0;
        habilitar_menu = false;
        inserir_senha_display(senha);
    }
    // Apagando a senha
    for (int i = 0; i < 4; i++)
    {
        senha[i] = '*';
    }

    acerto = 0;
    id_led = 0;
    criando_senha = false;
    gpio_put(LED_RED, true);
    gpio_put(LED_GREEN, true);
    gerencia_matriz_leds(valor_led, pio, sm, r, g, b, id_led, digito_atual, bloqueio);
    return 0;
}
// Função de interrupção para todos os botões A, B e SWITCH, em caso de acionamento retorna uma flag
void btns_callback(uint gpio, uint32_t events)
{
    current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    // Verifica o debounce
    if (current_time - last_time > DEBOUNCE_TIME)
    {
        if (gpio == BTN_A)
        {
            btnA_pressionado = true;
        }
        else if (gpio == BTN_B)
        {
            btnB_pressionado = true;
        }
        else if (gpio == SWITCH)
        {
            switch_pressionado = true;
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    }
}
// Retorna o sistema para o menu inicial, semelhante a função "reseta_ciclo_alarm_callback()", mas é chamada em momentos específicos
// Normalmente chamada ao mover o joystick para direita ou após o acionamento dos botões do webserver, para voltar ao MENU
// Ela altera as variáveis para seus estados iniciais ou equivalente para habilitar o MENU
void voltarPara_menu()
{
    for (int i = 0; i < 4; i++) // Zera a senha
    {
        senha[i] = '*';
    }
    for (int i = 0; i < 4; i++) // Zera a senha auxiliar
    {
        senha_aux[i] = '*';
    }
    habilitar_menu = true;
    menu_option = 1;
    digito_atual = 5;
    id_led = 0;
    init_i2c();
    options_menu();
    criando_senha = false;
    gpio_put(LED_RED, true);
    gpio_put(LED_GREEN, true);
    gerencia_matriz_leds(valor_led, pio, sm, r, g, b, id_led, digito_atual, bloqueio);
}
// Manipula as senhas, insere digitos A ou B na senha, fazendo distinção se está criando ou não a senha, basicamente percorre a senha incremento o digito_atual
void atualizar_senha(char letra)
{
    if (digito_atual <= 3) // Só insere se estiver dentro dos limites(4 digitos) da senha
    {
        if (criando_senha)
        {
            senha_aux[digito_atual] = letra; // variavel auxiliar que depois é passada para a variavel senha_correta;
            criar_senha_display(senha_aux);
        }
        else
        {
            senha[digito_atual] = letra;
            inserir_senha_display(senha);
        }
    }
    if (digito_atual <= 3) // Só incrementa se estiver dentro dos limites(4 digitos) da senha
    {
        digito_atual++;
    }
}
// Chamada ao acionar o SWITCH(enter), ela verifica se a senha inserida pelo usuário é equivalente com a senha correta do sistema
// Ela conta os acertos e em caso de 4, o acesso é liberado, após de uns segundo o sistema é reiniciado, armazena e verifica(var bloqueio) também quantas vezes o usuário errou
// A senha, em caso de 3 erros, o modo de bloqueio é acionado e após uns segundos o sistema é reiniciado por meio da função reseta_ciclo_alarm_callback()
void verifica_senhas()
{
    acerto = 0;
    for (int i = 0; i < 4; i++) // verificando se as senhas correspondem
    {
        if (senha_correta[i] == senha[i])
        {
            acerto++;
        }
    }
    if (acerto == 4) // Caso as senhas correspondam
    {
        gpio_put(LED_RED, false);
        id_led = 1;
        add_alarm_in_ms(6000, reseta_ciclo_alarm_callback, NULL, false);
    }
    else // Caso as senhas NÃO correspondam
    {
        bloqueio++;
        gpio_put(LED_GREEN, false);
        id_led = 2;
        if (bloqueio == 3) // Acionamento do modo bloqueio
        {
            add_alarm_in_ms(6000, reseta_ciclo_alarm_callback, NULL, false);
        }
        else
        {
            add_alarm_in_ms(2500, reseta_ciclo_alarm_callback, NULL, false);
        }
    }
}
// Verifica as FLAGS dos botões fornecidas pelas interrupções e chama a função "atualizar_senha()" para fazer a inserção passando como parâmetro a letra correspondente
// Ao botão e também habilita as funções atreladas ao SWITCH(enter) em caso da criação de senha ou verificar se as senhas correspondem
void verificar_botoes()
{
    if (btnA_pressionado)
    {
        atualizar_senha('A');
        btnA_pressionado = false;
    }
    if (btnB_pressionado)
    {
        atualizar_senha('B');
        btnB_pressionado = false;
    }
    if (switch_pressionado)
    {
        if (criando_senha)
        {
            for (int i = 0; i < 4; i++) // Ao confirmar a nova senha, a senha_correta recebe de senha_aux seus dígitos
            {
                senha_correta[i] = senha_aux[i];
            }
            senha_alterada_display();                                        // Display confirmando que a alteração de senha foi um sucesso
            add_alarm_in_ms(2500, reseta_ciclo_alarm_callback, NULL, false); // Reseta o sistema após criação da senha
        }
        else
        {
            verifica_senhas(); // Verifica se as senhas inseridas pelo usuário e a correta são iguais
        }
        switch_pressionado = false; // Reseta a flag
    }
}
// Possibilita o movimento horizontal do joystick durante os momentos de manipulação das senhas
void joystick_senhas(char senha[])
{
    leitura_joystick_Y(&valor_Y);
    if ((valor_Y / 100) < 11) // joystick para ESQUERDA APAGA a senha inserida
    {
        for (int i = 0; i < 4; i++)
        {

            senha[i] = '*';
        }

        digito_atual = 0;

        inserir_senha_display(senha);
    }
    else if ((valor_Y / 100) > 30) // joystick para DIREITA RETORNA ao menu
    {
        voltarPara_menu();
    }
}
// Configuração geral do sistema, reúne a inicialização das interfaces e perífericos, MENOS do WIFI
void setup()
{
    stdio_init_all();
    init_gpio();
    init_pio_matriz();
    init_i2c();
    setup_joystick();
}

int main()
{
    setup();
    inicia_web();
    // Interrupções configuradas para borda de descida, pois os botões estão configurados com PULL_UP
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &btns_callback);
    gpio_set_irq_enabled_with_callback(SWITCH, GPIO_IRQ_EDGE_FALL, true, &btns_callback);

    while (true)
    {
        if (sistema_status == true) // verifica a flag manipulada pelo WEBSERVER
        {
            if (acesso_direto == true && block_web == false) // verifica se o acesso direto foi acionado pelo WEBSERVER
            {
                // alegorias para sinalizar que o acesso direto foi habilitado
                alegorias_web();
                if (acesso_direto == false) // Quando for desativado o modo de acesso direto retorna ao menu
                {
                    voltarPara_menu();
                }
            }
            else if (acesso_direto == false && block_web == true) // verifica se o modo bloqueio foi acionado pelo WEBSERVER
            {
                // alegorias para sinalizar que o modo bloqueio foi habilitado
                alegorias_web();
                if (block_web == false) // Quando for desativado o modo de bloqueio retorna ao menu
                {
                    voltarPara_menu();
                }
            }
            else if (acesso_direto == false && block_web == false) // Quando tanto o acesso direto quanto o modo bloqueio estão desabilitados a execução "padrão" do sistema acontece
            {
                if (habilitar_menu == true) // verifica se o menu foi habilitado e libera o movimento vertical do joystick em caso positivo
                {
                    leitura_joystick_X(&valor_X); // Função para ler o joystick, divido por 100 para facilitar a manipulação/verificação
                    menu_display(valor_X);        // o valor X na placa é o vertical em relação ao display
                }
                else // se o habilitar_menu for falso, o sistema desabilita o movimento vertical do joystick e entende que o usuário acessou alguma das opções do menu
                {
                    if (menu_option == 3) // esse é o caso do acesso da opção "como usar" que o sistema apenas deixa o usuário retornar movendo o joystick para direita
                    {
                        leitura_joystick_Y(&valor_Y);
                        if ((valor_Y / 100) > 30)
                        {
                            voltarPara_menu();
                        }
                    }
                    else // esse é o caso do acesso das opções "inserir senha" e "criar senha"
                    {
                        verificar_botoes(); // nesse caso, o sistema habilita os botões A, B e SWITCH

                        if (criando_senha == false) // esse é o caso do usuário INSERIR uma senha, permite o movimento horizontal do joystick
                        {
                            joystick_senhas(senha);
                        }
                        else // esse é o caso do usuário CRIAR uma senha, permite o movimento horizontal do joystick
                        {
                            joystick_senhas(senha_aux);
                        }
                    }
                }
                // Atualiza a matriz de leds de acordo as var id_led, digito_atual e bloqueio
                gerencia_matriz_leds(valor_led, pio, sm, r, g, b, id_led, digito_atual, bloqueio);
            }
        }
        else // Caso o sistema seja DESATIVADO através do WEBSERVER
        {
            gpio_put(LED_GREEN, 0);
            sistemaOFF_display();
            id_led = 2;
            if (sistema_status == true) // Quando o sistema é ativado o sistema volta para o MENU
            {
                voltarPara_menu();
            }
            // Atualiza a matriz de leds de acordo as var id_led, digito_atual e bloqueio
            gerencia_matriz_leds(valor_led, pio, sm, r, g, b, id_led, digito_atual, bloqueio);
        }
        sleep_ms(10);
    }
}
