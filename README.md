# Trava Inteligente - Controle de acesso com RP2040

Este projeto implementa um **cofre eletrônico/controle de acesso simulado** no qual o usuário pode navegar por um menu, inserir uma senha, alterar a senha do sistema e controlar remotamente a trava por meio do web server, caso esteja na mesma rede Wi-Fi, usando um navegador o usuário pode acessar o IP do sistema e controlá-lo. Os botões foram tratados utilizando pull up, IRQ e debounce.
## Vídeo de apresentação: https://www.youtube.com/watch?v=hze41JATdF4
## Vídeo explicando a versão INICIAL do projeto: https://youtu.be/WcVbs2z3kEM

## Funcionalidades

- **Simulação de um cofre eletrônico** com verificação de senha.
- **Botão A** (GPIO5): Insere o número "1" na sequência.
- **Botão B** (GPIO6): Insere o número "2" na sequência.
- **Joystick** (GPIO22): Confirma a senha inserida.
- **Joystick para DIREITA** (GPIO26): Volta para o menu.
- **Joystick para ESQUERDA** (GPIO27): Apaga os caracteres digitados.
- **Senha pré-configurada** de 4 dígitos (AABB).
- **Tentativas limitadas**: Após 3 erros consecutivos, o sistema entra em modo de bloqueio.
- **Modo de bloqueio**: Aciona o alarme.
- **Desbloqueio bem-sucedido**: Se a senha for inserida corretamente, um padrão em verde é acionado.
- **Feedback sonoro**: Utilização de **buzzer(GPIO21)** para indicar o modo bloqueio.
- **Utilização de interrupção e polling** para leitura eficiente dos botões.
- **Temporização** para controle do modo bloqueio.
- **Matriz de leds (ws2812)**: (GPIO 7) Para resposta visual com usuário, informando dígito pressionado e status do sistema.
- **Menu de navegação**: O menu possui 3 opções(inserir senha, criar senha, como usar).
- **Inserir senha**: Usuário insere a senha e confirma para verificação.
- **Criar senha**: Permite ao usuário alterar a senha correta e visualizar a senha atual do sistema.
- **Como usar**: Tela com instruções para utilização do sistema.
- **Conexão Wi-Fi**: CY43.
- **BOOTSEL VIA WEB SERVER**: Entra no **modo Bootsel**.
- **Web Server**: Comunicação com outros dispositivos via HTTP, acessando o IP da placa na mesma rede Wi-Fi, o usuário pode ligar ou desligar: o sistema, o modo bloqueio (emergência) e o acesso direto (Destrava a tranca sem necessidade de senha).

## Tecnologias Utilizadas

- **Microcontrolador**: Raspberry Pi Pico W (RP2040)
- **Linguagem de Programação**: C (usando o **Pico SDK**). obs: há um script python de tradução do html para hexa e crinado o arquivo.c com informações para o web server.
- **Ambiente de Desenvolvimento**: Visual Studio Code (VSCode) com Pico SDK
- **Hardware**:
  - 2 Botões físicos (GPIO5) e (GPIO6)
  - LED RGB (GPIO11) e (GPIO13)
  - Buzzer (GPIO21)
  - Display SSD1306 OLED (GPIO14 SDA), (GPIO15 SCL) e endereço 0x3c
  - Matriz de leds ws2812 (GPIO7)
  - Joystick analógico (GPIO22), (GPIO26) e (GPIO27)

## Como Usar

obs: certifique-se de ter alterado as informações para sua rede Wi-Fi 
1. Compile e carregue o código no Raspberry Pi Pico.
2. O sistema tenta conectar a rede Wi-Fi.
3. Após se conectar ao Wi-Fi o sistema habilita o menu com as 3 opções.
4. O usuário pode mover o joystick na vertical para navegar entre as opções e pressionar o mesmo para confirmar a seleção
5. Nas duas primeiras opções("Inserir senha" e "Criar senha") do menu, o usuário pode digitar a senha utilizando os botões A e B, além de poder apagar os mesmos dígitos movendo o joystick para esquerda e retornar ao menu movendo-o para direita.
6. Na terceira opção o usuário encontra uma tela com instruções de uso do sistema.

## Web Server ##

## Como Acessar:
  - Ao conectar-se no Wi-Fi o programa imprime no terminal o seu IP (Você pode encontrá-lo de outras maneiras na sua rede também).
  - Usando um navegador busque pelo IP da placa, logo você será capaz de utilizá-lo.
**Como Alterar o Site (HTML)**:

  - Você deve alterar o arqivo index.shtml dentro da pasta html_files como um arquivo html comum.
  - Após isso será necessário rodar o script makefsdata.py -> no terminal do vscode mesmo você pode executá-lo com esse comando: python makefsdata.py (RECOMENDO QUE APAGUE O ARQUIVO htmldata.c ANTES DE RODAR O SCRIPT).
  - O script vai gerar um novo htmldata.c, basicamente ele converte o arquivo HTML para hexadecimal e traduz em um arquivo .c com as informações para o html.
    
**Como Passar a Lógica dos botões**:
  - Você deve manipular o arquivo cgi.h criando um novo handler seguindo o template que você pode encontrar nesse projeto, porém altere a variável e o que a chamada do botão fará, no meu caso são FLAGS e o botão do bootsel entra no modo de gravação.
  - Deve-se alterar o arquivo html passando aos botões links referentes às chamadas das funções, exemplo a href="/led.cgi?led=toggle, faz com que ao apertar o botão o estado do led seja alterado.

**Como alterar a Exibição das Variáveis**
  - Você deve manipular o arquivo ssi.h criando uma nova tag dentro de const char *ssi_tags[]
  - No handler deve-se criar um novo case no switch, o case é a posição da tag dentro de ssi_tags
  - printed = snprintf(pcInsert, iInsertLen, "ON"); Aqui você vai definir o que irá ser exibido de acordo a sua lógica ao utilizar a tag, nesse caso irá aparecer no html ON quando eu chamar tag block e ela estiver ativada.
  - No arquivo html é assim que você utiliza a tag por exemplo <p>STATUS DA EMERGÊNCIA: (<!--#block-->)</p>

