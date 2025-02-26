#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "setup_display.h"

// Manipulador CGI para controle do LED
const char *cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    for (int i = 0; i < iNumParams; i++) {
        if (strcmp(pcParam[i], "led") == 0) {
            // Alterna o estado do LED entre ligado e desligado
            sistema_status = !sistema_status;
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, sistema_status ? 1 : 0);
            break; // Interrompe após encontrar o parâmetro "led"
        }
    }
    return "/index.shtml"; // Retorna para a página principal
}

// Manipulador CGI para controle de bloqueio do sistema via web
const char *cgi_block_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Verifica se o parâmetro "block" foi recebido
    if (strcmp(pcParam[0], "block") == 0) {
        // Alterna o estado da variável block_web
        block_web = !block_web;
    }
    return "/index.shtml"; // Retorna para a página principal
}

// Manipulador CGI para controle do acesso direto
const char *cgi_acesso_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Verifica se o parâmetro "acesso" foi recebido
    if (strcmp(pcParam[0], "acesso") == 0) {
        acesso_direto = !acesso_direto;
    }
    return "/index.shtml"; // Retorna para a página principal
}

// Manipulador CGI para acionar o modo BOOTSEL do RP2040
const char *cgi_bootsel_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Verifica se o parâmetro "bootsel" foi recebido
    if (strcmp(pcParam[0], "bootsel") == 0) {
        reset_usb_boot(0, 0); // Reinicia o microcontrolador no modo USB BOOTSEL
    }
    return "/index.shtml"; // Retorna para a página principal
}

// Definição dos manipuladores CGI disponíveis no servidor
static const tCGI cgi_handlers[] = {
    {"/led.cgi", cgi_led_handler},
    {"/block.cgi", cgi_block_handler},
    {"/acesso.cgi", cgi_acesso_handler},
    {"/bootsel.cgi", cgi_bootsel_handler}
};

// Inicializa os manipuladores CGI no servidor HTTP
void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, sizeof(cgi_handlers) / sizeof(tCGI));
}
