#ifndef BATERIA_H
#define BATERIA_H
#include <stdint.h>

/**
 * @file
 * @brief Biblioteca para a utilização de um sensor de tensão.
 *
 * Este arquivo contém as definições e funções necessárias para inicializar 
 * e ler dados do sensor de tensão desenvolvido pela equipe UNICAMP Baja SAE
 *
 * @author Giancarlo Bonvenuto e Matheus Cunha
 */


/**
 * @brief Valor multiplicado ao valor do `bateria_read()` para transformá-lo de
 * max 3.3V para max 12V
 */
extern const double calibration_factor;

/**
 * @brief Inicializa o Analog to Digital Converter (ADC), e os pinos do adc e da
 * base do transistor. Deve ser chamado antes de qualquer função dessa biblioteca
 */
int bateria_init();

/**
 * @brief Ativa o transistor, espera alguns milisegundos e em seguida lê os sinal
 * do trimpot que indica a tensão. E por fim desativa o transitor.
 * 
 * @return 
 * - `>= 0`: caso a leitura seja bem-sucedida. O valor é retornado em milivolts
 * - `< 0`: retorna o erro que ocorreu em alguma das funções executadas
 */
uint32_t bateria_read();

#endif // !BATERIA_H
