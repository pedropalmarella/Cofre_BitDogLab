#ifndef SETUP_DISPLAY_H
#define SETUP_DISPLAY_H

#include "hardware/i2c.h"
#include "init_adc.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C // Endereço do display SSD1306

ssd1306_t ssd; // Estrutura do display OLED
bool cor = false; // Cor padrão do display (preto e branco invertido)

// Variáveis de controle do sistema manipuladas no WEBSERVER
bool block_web = false;
bool sistema_status = true;
bool acesso_direto = false;
int acerto = 0; // Contador de dígitos corretos na senha
//FLAGS para confirmar o estado que o usuário se encontra
bool inserindo_senha = false;
bool criando_senha = false;
int menu_option = 1;
bool habilitar_menu = true;

// Senhas utilizadas no sistema
char senha[5] = {'*', '*', '*', '*', '\0'};
char senha_aux[5] = {'*', '*', '*', '*', '\0'};
char senha_correta[5] = {'A', 'A', 'B', 'B', '\0'};

int digito_atual = 5; // Índice do dígito da senha sendo inserido
int id_led = 0;   // Estado da matriz
int bloqueio = 0; // Contador de tentativas erradas de senha

#define DEBOUNCE_TIME 300000 // Tempo de debounce (300ms em microssegundos)
uint32_t current_time = 0; // Armazena o tempo que foi acionado para debounce
uint32_t last_time = 0; // Armazena o tempo do último acionamento para debounce

// Função para exibir o menu principal
void options_menu()
{
    ssd1306_fill(&ssd, cor);          
    ssd1306_draw_string(&ssd, "* MENU *", 29, 3);       
    ssd1306_draw_string(&ssd, "INSERIR SENHA", 10, 20); 
    ssd1306_draw_string(&ssd, "CRIAR SENHA", 20, 37);   
    ssd1306_draw_string(&ssd, "COMO USAR", 25, 53);     
    // Destaca a opção selecionada, informando visualmente com o retângulo para o usuário
    if(menu_option == 1){
        ssd1306_rect(&ssd, 16, 3, 122, 15, !cor, cor);
    }
    else if(menu_option == 2){
        ssd1306_rect(&ssd, 33, 3, 122, 15, !cor, cor);
    }
    else if(menu_option == 3){
        ssd1306_rect(&ssd, 49, 3, 122, 15, !cor, cor);
    }
    ssd1306_send_data(&ssd);
}
// Inicializa o barramento I2C e o display
void init_i2c()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_fill(&ssd, cor);                                      // Limpa o display
    ssd1306_rect(&ssd, 16, 3, 122, 15, !cor, cor);
}
// Exibe a tela de inserção de senha
void inserir_senha_display(char senha[])
{
    ssd1306_fill(&ssd, cor); 
    ssd1306_rect(&ssd, 26, 35, 50, 15, !cor, cor);
    ssd1306_draw_string(&ssd, "A SENHA TEM 4", 10, 10); 
    ssd1306_draw_string(&ssd, senha, 45, 30);           
    ssd1306_draw_string(&ssd, "DIGITOS A ou B", 8, 48); 
    ssd1306_send_data(&ssd);
}
// Exibe a tela de criação de senha
void criar_senha_display(char senha_aux[])
{
    ssd1306_fill(&ssd, cor); 
    ssd1306_rect(&ssd, 23, 35, 50, 15, !cor, cor);
    ssd1306_draw_string(&ssd, "NOVA SENHA", 24, 8);   
    ssd1306_draw_string(&ssd, senha_aux, 45, 27); 
    ssd1306_draw_string(&ssd, "SENHA ATUAL", 17, 44); 
    ssd1306_draw_string(&ssd, senha_correta, 45, 55);    
    ssd1306_send_data(&ssd);
}
// Exibe a confirmação de alteração de senha
void senha_alterada_display()
{
    ssd1306_fill(&ssd, cor);                            
    ssd1306_draw_string(&ssd, "SENHA ALTERADA", 8, 22); 
    ssd1306_draw_string(&ssd, "COM SUCESSO", 20, 42);   
    ssd1306_send_data(&ssd);
}
// Exibe a tela indicando que o sistema está desligado
void sistemaOFF_display()
{
    ssd1306_fill(&ssd, cor);
    ssd1306_draw_string(&ssd, "SISTEMA *OFF*", 8, 32);
    ssd1306_send_data(&ssd);
}
// Exibe a tela inicial tentando conectar a rede
void conectando_rede_display()
{
    ssd1306_fill(&ssd, cor);                            
    ssd1306_draw_string(&ssd, "*CARREGANDO*", 2, 15); 
    ssd1306_draw_string(&ssd, "CONECTANDO", 2, 35);   
    ssd1306_draw_string(&ssd, "A REDE", 2, 55);   
    ssd1306_send_data(&ssd);
}
// Exibe a tela de acesso direto ativado
void acesso_direto_display()
{
    ssd1306_fill(&ssd, cor);                            
    ssd1306_draw_string(&ssd, "ACESSO LIVRE", 2, 25); 
    ssd1306_draw_string(&ssd, "ATIVADO",        10, 45);   
    ssd1306_send_data(&ssd);
}
// Exibe a tela de emergência
void emergencia_display()
{
    ssd1306_fill(&ssd, cor);                            
    ssd1306_draw_string(&ssd, "BLOQUEIO ATIVO", 2, 15); 
    ssd1306_draw_string(&ssd, "AS AUTORIDADES", 2, 35);   
    ssd1306_draw_string(&ssd, "FORAM ACIONADAS", 2, 55);   
    ssd1306_send_data(&ssd);
}
// Exibe o menu de instruções
void como_usar_display(){
    ssd1306_fill(&ssd, cor);
    ssd1306_draw_string(&ssd, "DIREITA *VOLTA", 0, 0);       
    ssd1306_draw_string(&ssd, "ESQUERDA *APAGA", 0, 20); 
    ssd1306_draw_string(&ssd, "MEIO *ENTER", 0, 38);   
    ssd1306_draw_string(&ssd, "A ou B *DIGITA", 0, 55);     
    ssd1306_send_data(&ssd);
}
// Lógica para navegação no menu
void menu_display(uint16_t valor_X)
{
    // debounce
    current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if (current_time - last_time > DEBOUNCE_TIME)
    {
        if ((valor_X / 100) > 24 && menu_option != 3) //Verifica se o joystick foi movido para cima e não está na última opção
        {
            menu_option = 1; // informa o sistema
            options_menu(); // informa o usuário visualmente atualizando o display
        }
        else if ((valor_X / 100) < 14 && menu_option == 1 || (valor_X / 100) > 24 && menu_option == 3) //Verifica se o joystick foi movido para cima ou para baixo e onde o retangulo estava
        {
            menu_option = 2; // informa o sistema
            options_menu(); // informa o usuário visualmente atualizando o display
        }
        else if ((valor_X / 100) < 14 && menu_option == 2) //Verifica se o joystick foi movido para baixo estava na opção do meio 2
        {
            menu_option = 3; // informa o sistema
            options_menu(); // informa o usuário visualmente atualizando o display
        }
        // Verifica se o SWITCH foi pressionado e depois a opção que o usuário havia definido, semelhante a um "ENTER" e em seguida a manipulação do menu é desabilitada
        if (!gpio_get(SWITCH))
        {
            if (menu_option == 1) // Caso a primeira opção foi escolhida, a tela de inserção será exibida
            {
                inserir_senha_display(senha);
                habilitar_menu = false;
                digito_atual = 0;
                inserindo_senha = true;
            }
            else if(menu_option == 2) // Caso a segunda opção foi escolhida, a tela de criação será exibida
            {
                for (int i = 0; i < 4; i++) // zera a senha que aparece no meio da tela
                {
                    senha_aux[i] = '*';
                }

                criar_senha_display(senha_aux);
                habilitar_menu = false;
                digito_atual = 0;
                criando_senha = true;
            }
            else if(menu_option == 3) // Caso a terceira opção foi escolhida, a tela de instrução será exibida
            {
                como_usar_display();
                habilitar_menu = false;
            }
        }
        last_time = current_time;
    }
}
#endif