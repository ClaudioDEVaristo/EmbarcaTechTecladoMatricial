#include <stdio.h>
#include "pico/stdlib.h"

#define gpio_led_red 18
#define gpio_led_green 19
#define gpio_led_blue 20
#define gpio_buzzer 21

// definição das colunas e das linhas e mapeamento do teclado
uint8_t coluna[4] = {4, 3, 2, 1};
uint8_t linha[4] = {8, 7, 6, 5};
char teclas[4][4] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

// Função para ativar e desligar leds
void piscar_led(uint gpio_led)
{                            // Função que faz o led piscar com base no GPIO passado
  gpio_put(gpio_led, true);  // Ligar o led
  sleep_ms(2000);            // Tempo de espera em milissegundos
  gpio_put(gpio_led, false); // Desligar o led
}
// função de inicializar o teclado
void inicializar_teclado(uint8_t *colunas, uint8_t *linhas)
{
  for (int i = 0; i < 4; i++)
  {
    gpio_init(linhas[i]);
    gpio_set_dir(linhas[i], GPIO_OUT);
    gpio_put(linhas[i], 1); // Inicialmente em HIGH

    gpio_init(colunas[i]);
    gpio_set_dir(colunas[i], GPIO_IN);
    gpio_pull_up(colunas[i]); // Habilita pull-up nas colunas
  }
}

// função de leitura do teclado
char ler_teclado(uint8_t *colunas, uint8_t *linhas)
{
  for (int i = 0; i < 4; i++)
  {
    gpio_put(linhas[i], 0);
    for (int j = 0; j < 4; j++)
    {
      if (!gpio_get(colunas[j]))
      {                         // Detectar coluna ativa
        gpio_put(linhas[i], 1); // Desativar linha
        sleep_ms(10);
        return teclas[i][j];
      }
    }
    gpio_put(linhas[i], 1);
  }
  return 0;
}

void pisca_led_branco()
{
  gpio_put(gpio_led_red, 1);
  gpio_put(gpio_led_green, 1);
  gpio_put(gpio_led_blue, 1);
  sleep_ms(2000);

  gpio_put(gpio_led_red, 0);
  gpio_put(gpio_led_green, 0);
  gpio_put(gpio_led_blue, 0);
}

void acionar_buzzer()
{
  gpio_put(gpio_buzzer, 1); // Liga o buzzer
  sleep_ms(2000);           // Emite som por 2000ms
  gpio_put(gpio_buzzer, 0); // Desliga o buzzer
}

int main()
{
  stdio_init_all();
  gpio_init(gpio_led_red);
  gpio_init(gpio_led_green);
  gpio_init(gpio_led_blue);
  gpio_init(gpio_buzzer);
  gpio_set_dir(gpio_led_red, GPIO_OUT);
  gpio_set_dir(gpio_led_green, GPIO_OUT);
  gpio_set_dir(gpio_led_blue, GPIO_OUT);
  gpio_set_dir(gpio_buzzer, GPIO_OUT);

  inicializar_teclado(coluna, linha);

  while (true)
  {
    char tecla_pressionada = ler_teclado(coluna, linha); // Atribui a variável a tecla pressionada ao chamar a função

    if (tecla_pressionada)
    {
      printf("Tecla pressionada: %c\n", tecla_pressionada);

      switch (tecla_pressionada)
      { // Analisa a tecla pressionada e executa a ação definido a ela
      case 'A':
        piscar_led(gpio_led_blue); // Verifica se a tecla A foi pressionada
        break;

      case 'B':                   // Verifica se a tecla B foi pressionada
        piscar_led(gpio_led_red); // Chama a funcao piscar_periferico
        break;

      case 'D': // Piscar luz branca uma vez
        pisca_led_branco();
        break;
      case '#': // Verifica se a tecla # foi pressionada
        acionar_buzzer();
        break;

      default:
        printf("Tecla não configurada\n");
      }
      sleep_ms(100); // Delay para evitar leitura repetida
    }
  }
}
