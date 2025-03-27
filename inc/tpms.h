#ifndef TPMS_H
#define TPMS_H

#include <zephyr/logging/log.h>

/**
 * @struct s_tpms 
 * @brief Representa o TPMS 
 */
extern struct s_tpms {
  /** Referência ao device tree que representa */
  const struct device *dev;
  /** Buffer onde serão registradas as mensagens do callback */
  char *buffer;
  /** Número de caracteres do buffer */
  int buff_len;
  /** Booleano que registra se o GPS está pronto para receber um novo dado */
  bool ready;

  int pressao[4];         
  int temperatura[4];          
  char information_status; // A: status OK
                           // V: status NOT ok

} tpms;

/**
 * @brief Inicializa o TPMS
 * 
 * @return int que representa algum erro no callback
 */
int init_tpms();

/**
 * @brief executa o read_line(), dessa forma o read_line não
 * segura o restante do código
 */
void tpms_tick();

#endif // TPMS_H
