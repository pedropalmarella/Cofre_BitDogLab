# Cofre Simulado com RP2040

Este projeto implementa um **cofre eletrônico simulado** utilizando o microcontrolador RP2040. O cofre é pré-configurado com uma senha de **4 dígitos**, onde o usuário insere valores utilizando dois botões. Se o usuário errar a senha **3 vezes seguidas**, o sistema entra em **modo de bloqueio**. Caso a senha seja inserida corretamente, o **cofre será desbloqueado** com um sinal visual piscando em **verde**.

## Funcionalidades

- **Simulação de um cofre eletrônico** com verificação de senha.
- **Botão A** (GPIO5): Insere o número "1" na sequência.
- **Botão B** (GPIO6): Insere o número "2" na sequência.
- **Joystick** (GPIO22): Confirma a senha inserida.
- **Botões A + B pressionados simultaneamente**: Entra no **modo Bootsel**.
- **Senha pré-configurada** de 4 dígitos.
- **Tentativas limitadas**: Após 3 erros consecutivos, o sistema entra em modo de bloqueio.
- **Modo de bloqueio**: Impede novas tentativas por um tempo determinado.
- **Desbloqueio bem-sucedido**: Se a senha for inserida corretamente, um padrão em verde é acionado.
- **Feedback sonoro**: Utilização de **buzzer(GPIO21)** para indicar acertos e erros.
- **Utilização de interrupção e polling** para leitura eficiente dos botões.
- **Temporização** para controle do modo bloqueio.

## Tecnologias Utilizadas

- **Microcontrolador**: Raspberry Pi Pico (RP2040)
- **Linguagem de Programação**: C (usando o **Pico SDK**)
- **Ambiente de Desenvolvimento**: Visual Studio Code (VSCode) com Pico SDK
- **Hardware**:
  - Botões físicos
  - LED indicador
  - Buzzer

## Instalação

### Requisitos

- **Visual Studio Code** instalado.
- **Pico SDK** configurado no seu ambiente de desenvolvimento.
- **CMake** instalado.

## Como Usar

1. Compile e carregue o código no Raspberry Pi Pico.
2. O sistema inicia pronto para receber a senha.
3. Use o **botão A** para inserir "1" e o **botão B** para inserir "2".
4. Pressione o **joystick** para confirmar a senha.
5. **Se a senha for correta**, um LED verde pisca indicando sucesso.
6. **Se a senha for errada**, o buzzer emite um som de erro.
7. Após **3 tentativas erradas**, o sistema entra em **modo de bloqueio** e impede novas tentativas por um tempo determinado.
8. Pressione **botão A + botão B simultaneamente** para entrar no **modo Bootsel**.
