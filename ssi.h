#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

// Definição das tags SSI - cada tag deve ter no máximo 8 bytes
const char *ssi_tags[] = {"block", "senha", "led", "tranca"};
// Manipulador do SSI (Server-Side Includes), passo essas tags para o HTML depois e é exibido no WEBSERVER
u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
  size_t printed;
  switch (iIndex)
  {
  case 0: // Tag "block" - indica o status da var no webserver
  {
    if (block_web)
    {
      printed = snprintf(pcInsert, iInsertLen, "ON");
    }
    else
    {
      printed = snprintf(pcInsert, iInsertLen, "OFF");
    }
  }
  break;
  case 1: // Tag "senha" - exibe a senha correta no webserver
  {
    printed = snprintf(pcInsert, iInsertLen, "%s", senha_correta);
  }
  break;
  case 2: // Tag "led" (status do sitema) - verifica o status do LED do sistema no webserver
  {
    bool led_status = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
    if (led_status == true)
    {
      printed = snprintf(pcInsert, iInsertLen, "ON");
    }
    else
    {
      printed = snprintf(pcInsert, iInsertLen, "OFF");
    }
  }
  break;
  case 3: // Tag "tranca" - verifica se a fechadura está aberta ou fechada, altera no webserver
  {
    if (acerto == 4 || acesso_direto == true)
    {
      printed = snprintf(pcInsert, iInsertLen, "ABERTA");
    }
    else
    {
      printed = snprintf(pcInsert, iInsertLen, "FECHADA");
    }
  }
  break;
  default: // Caso não seja nenhuma das tags conhecidas
    printed = 0;
    break;
  }

  return (u16_t)printed;
}

// Inicializa o manipulador SSI
void ssi_init()
{
  // Registra o manipulador SSI no servidor web
  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
